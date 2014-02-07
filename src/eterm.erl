-module(eterm).
-behaviour(gen_server).

-export([
    start_link/0,
    send/2,
    close/1
]).

%% gen_server callbacks
-export([
    init/1,
    handle_call/3,
    handle_cast/2,
    handle_info/2,
    terminate/2,
    code_change/3]).

-record(state, {
    port
}).

-define(L(_F,_A), io:format(user, "[~p:~p] "_F, [?MODULE, ?LINE | _A])).

start_link() ->
    case gen_server:start_link(?MODULE, [], []) of
        {ok, Pid} -> {?MODULE, Pid};
        Error -> Error
    end.

close({?MODULE, PortPid}) -> gen_server:call(PortPid, close, infinity).
send(Term, {?MODULE, PortPid}) -> gen_server:call(PortPid, {send, Term}, infinity).

%% Callbacks
init(_) ->
    PrivDir = case code:priv_dir(eterm) of
        {error,_} -> "./priv/";
        PDir -> PDir
    end,
    case os:find_executable("eterm", PrivDir) of
        false ->
            case os:find_executable("eterm", "./deps/eterm/priv/") of
                false -> {stop, bad_executable};
                Executable ->
                    start_exe(Executable)
            end;
        Executable ->
            start_exe(Executable)
    end.

start_exe(Executable) ->
    PortOptions = [ {packet, 4}
                  , binary
                  , exit_status
                  , use_stdio
                  ],
    case (catch open_port({spawn_executable, Executable}, PortOptions)) of
        {'EXIT', Reason} ->
            ?L("oci could not open port: ~p", [Reason]),
            {stop, Reason};
        Port ->
            {ok, #state{port=Port}}
    end.

handle_call(close, _From, #state{port=Port} = State) ->
    try
        erlang:port_close(Port)
    catch
        _:R -> error_logger:error_report("Port close failed with reason: ~p~n", [R])
    end,
    {stop, normal, ok, State};
handle_call({send, Msg}, _From, #state{port=Port} = State) ->
    BTerm = term_to_binary(Msg),
    true = port_command(Port, BTerm),
    {reply, ok, State}.

handle_cast(Msg, State) ->
    ?L("Received unexpected cast: ~p~n", [Msg]),
    {noreply, State}.

%% We got a reply from a previously sent command to the Port.  Relay it to the caller.
handle_info({Port, {data, Data}}, #state{port=Port} = State) when is_binary(Data) andalso (byte_size(Data) > 0) ->    
    Resp = binary_to_term(Data),
    ?L("Data ~p~n", [Resp]),
    {noreply, State};
handle_info({Port, {exit_status, Status}}, #state{port = Port} = State) ->
    ?L("port ~p exited with status ~p~n", [Port, Status]),
    {stop, normal, State};
%% Catch all - throws away unknown messages (This could happen by "accident"
%% so we do not want to crash, but we make a log entry as it is an
%% unwanted behaviour.)
handle_info(Info, State) ->
    ?L("Received unexpected info: ~p~n", [Info]),
    {noreply, State}.

terminate(Reason, #state{port=Port}) ->
    ?L("Terminating ~p: ~p", [{self(), Port}, Reason]),
    catch port_close(Port),
    ok.

code_change(_OldVsn, State, _Extra) ->
    {ok, State}.
