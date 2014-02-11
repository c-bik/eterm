eterm
=====

Erlang binary term STL wrapper

erl_interface <-> stdin/stdout
------------------------------
 * for a {packet, 4} binary port, port_close sends 0x0, 0x0, 0x0, 0x0 sequence to shutdown the port
 * [linux] stdin/stdout are buffered, FD '0' for read and '1' to write is used instead
 * [win] _fileno(stdin/out) used to get the FD for stdin and stdout
 * read/write used uniformly in platform independent way
 * read / write (FD access) are protected with port_io_lock mutex to serialize buffer read and write (multi threading protection)
 * encode / decode also protected with transcoder_lock mutex to make erl_interface library portable between single and multithread environment
