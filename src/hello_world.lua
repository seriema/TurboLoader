
bitmaps {
	jp = "./res/jp.png",
	jb = "./res/jb.png",
	img_test_bmp = "./res/img_test.bmp",
	img_test_dds = "./res/img_test.dds",
}

shaders {
	debug = "./res/debug",
}

gui {
	{ id = "boxart", kind = "bitmap", pos = { 0, 0 }, shader = "debug", data = "img_test_bmp" },
	--{ id = "boxart", kind = "bitmap", pos = {400,200}, data = "${boxart}" },
}
