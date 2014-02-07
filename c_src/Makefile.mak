all:
	vcbuild /platform:x64 /logcommands c_src/eterm/eterm.vcproj debug
	
clean:
	vcbuild /clean /logcommands c_src/eterm/eterm.vcproj debug
