#include <gtk/gtk.h>
#include <libayatana-appindicator/app-indicator.h>

#ifdef WITHX11
	#include <X11/XKBlib.h>
	#include <X11/Xlib.h>
	Display *d;
	XkbStateRec xkbcapstate;
	XkbStateRec xkbnumstate;
#else
	#include <fcntl.h>
	#include <libinput.h>
	#include <libevdev/libevdev.h>
	#include <grp.h>
	#include <poll.h>
		struct libinput *libinput;
	#include "evdevintegrity.h"

#endif

#include <pthread.h>

#define ML 256

#define BUFFER_SIZE 1024

GtkIconTheme *theme;
GtkIconInfo *info;
GdkPixbuf *icon;

AppIndicator *capindicator, *numindicator;

GtkWidget *capmenu, *capmenu_item_toggle,*capmenu_item_settings, *capmenu_item_about, *capmenu_item_quit,
*nummenu, *nummenu_item_toggle,*nummenu_item_settings, *nummenu_item_about, *nummenu_item_quit,

*window, *gshowcap, *gshownum, *gupdrate, *gnumstate, *gcapstate, *dialog, *btn_savesettings, *grid, *btn_cancel;

pthread_t cap_threadid, num_threadid;
guint capstate=0, numstate=0, vcapstate=0, vnumstate=0, showcap=1, shownum=1, updrate=1, nohome=0, capvisible, numvisible, isx11;

gchar *pver="1.2", config_file_path[ML];

#include "threadmgr.h"
#include "keyhandler.h"
#include "settings.h"
#include "cfgmgr.h"
#include "mainwindow.h"
