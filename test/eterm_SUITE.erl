-module(eterm_SUITE).
-export([all/0, init_per_suite/1, end_per_suite/1]).
-export([load/1]).

-include_lib("common_test/include/ct.hrl").

-define(value(Key,Config), proplists:get_value(Key,Config)).

all() -> [load].

init_per_suite(InitConfigData) -> InitConfigData.

end_per_suite(ConfigData) ->
    ct:pal(info, "Finishing...", []).

load(ConfigData) ->
    Et = eterm:start_link(),
    parallel_call(Et),
    Et:close().

parallel_call(Et) ->
    Count = 50000,
    Self = self(),
    random:seed(erlang:now()),
    Terms = [1, 1.2, atom, "a string", <<"a binary">>, {"a tuple"}, ["a list"], self(), make_ref()],
    Ids = [random:uniform(length(Terms)) || _I <- lists:seq(1,Count)],

    ct:pal(info, "Spawning ~p processes to send term randomly from ~p~n", [Count, Terms]),
    Recv = spawn(fun() -> receiver(Self, Count, 1) end),
    [spawn(fun() ->
             Term = lists:nth(I, Terms),
             Term = Et:send(Term),
             %ct:pal(info, "-> ~p~n", [self()]),
             Recv ! {self(), ok}
           end) || I <- Ids],
    receive
        done ->
           ct:pal(info, "All ~p processes finished~n", [Count])
    end.

receiver(Master, Count, SoFar) ->
    receive
        {_Pid, ok} ->
            %ct:pal(info, "<- ~p ~p/~p~n", [_Pid, SoFar, Count]),
            if Count =:= SoFar -> Master ! done;
               true -> receiver(Master, Count, SoFar+1)
            end
    end.
