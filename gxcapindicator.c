#include "gxcapindicator.h"

int main(int argc, char *argv[])
{
	readconf();

	#ifdef WITHX11
		g_print("Running with X11 backend");
	#else
		g_print("Running with Evdev backend");

		check_permissions("input");
		check_udev();
	#endif

	gtk_init(&argc, &argv);
	create_window();
}
