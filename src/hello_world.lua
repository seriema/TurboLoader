
local argc, argv = lua_hello_world( 13 )

print( "Argc: ", argc )
print( "Argv: ", argv )

bitmaps {
	img_test_bmp = "./res/img_test.bmp",
	img_test_dds = "./res/img_test.dds",
}

shaders {
	debug = "./res/debug",
}
