#ifndef WITHX11
// Event printing functions
static int print_key_event(struct libinput_event *event)
{
	struct libinput_event_keyboard *keyboard = libinput_event_get_keyboard_event(event);

	guint32 key_code = libinput_event_keyboard_get_key(keyboard);
	enum libinput_key_state state_code = libinput_event_keyboard_get_key_state(keyboard);

	const guint32 KEY_CAPSLOCK_CODE = KEY_CAPSLOCK;
	const guint32 KEY_NUMLOCK_CODE = KEY_NUMLOCK;

	if (showcap && key_code == KEY_CAPSLOCK_CODE && state_code == LIBINPUT_KEY_STATE_RELEASED)
	{
		capstate ^= 1;
		const gchar *icon_name = capstate ? "keyboard-caps-enabled" : "keyboard-caps-disabled";
		const gchar *status_message = capstate ? "Caps Lock: Enabled" : "Caps Lock: Disabled";
		app_indicator_set_icon_full(capindicator, icon_name, status_message);
		app_indicator_set_title(capindicator, status_message);
	}
	else if (shownum && key_code == KEY_NUMLOCK_CODE && state_code == LIBINPUT_KEY_STATE_RELEASED)
	{
		numstate ^=1;
		const gchar *icon_name = numstate ? "keyboard-num-enabled" : "keyboard-num-disabled";
		const gchar *status_message = numstate ? "Num Lock: Enabled" : "Num Lock: Disabled";
		app_indicator_set_icon_full(numindicator, icon_name, status_message);
		app_indicator_set_title(numindicator, status_message);
	}
	return 0;
}


static int handle_events(struct libinput *input)
{
	struct libinput_event *event;

	if (libinput_dispatch(input) < 0)
		return -1;

	while ((event = libinput_get_event(input)) != NULL)
	{
		switch (libinput_event_get_type(event))
		{
			case LIBINPUT_EVENT_KEYBOARD_KEY:
				print_key_event(event);
				break;
			default:
				break;
		}
		libinput_event_destroy(event);
	}
	return 0;
}

static int run_mainloop(struct libinput *input)
{
	struct pollfd fd;
	fd.fd = libinput_get_fd(input);
	fd.events = POLLIN;

	while (poll(&fd, 1, -1) > -1)
		handle_events(input);
	return 0;
}
#endif

void *toggle_cap(void)
{
	#ifdef WITHX11
	XkbGetIndicatorState(d, XkbUseCoreKbd, &capstate);
	int capsLock = (capstate & 0x01) != 0;
	XkbLockModifiers(d, XkbUseCoreKbd, LockMask, capsLock ? 0 : LockMask);
	#else
	g_warning("not, implemented");
	#endif
	return NULL;
}

void *check_cap(void* arg)
{
	#ifdef WITHX11
		XkbStateRec xkbState;
		while (1)
		{
			XkbGetState(d, XkbUseCoreKbd, &xkbState);
			capstate = (xkbState.locked_mods & LockMask) ? 1 : 0;
			const gchar *icon_name = capstate ? "keyboard-caps-enabled" : "keyboard-caps-disabled";
			const gchar *status_message = capstate ? "Caps Lock: Enabled" : "Caps Lock: Disabled";
			app_indicator_set_icon_full(capindicator, icon_name, status_message);
			app_indicator_set_title(capindicator, status_message);
			sleep(updrate);
		}
	#else
		run_mainloop(libinput);
	#endif
	return NULL;
}

void *toggle_num(void)
{
	#ifdef WITHX11
	XkbGetIndicatorState(d, XkbUseCoreKbd, &numstate);
	int numLock = (numstate & 0x02) != 0;
	XkbLockModifiers(d, XkbUseCoreKbd, Mod2Mask, numLock ? 0 : Mod2Mask);
	#else
	g_warning("not, implemented");
	#endif
	return NULL;
}

void *check_num(void *arg)
{
#ifdef WITHX11
	while (1)
	{
		XkbGetState(d, XkbUseCoreKbd, &xkbnumstate);
		numstate = (xkbnumstate.locked_mods & Mod2Mask) ? 1 : 0;
		const gchar *icon_name = numstate ? "keyboard-num-enabled" : "keyboard-num-disabled";
		const gchar *status_message = numstate ? "Num Lock: Enabled" : "Num Lock: Disabled";
		app_indicator_set_icon_full(numindicator, icon_name, status_message);
		app_indicator_set_title(numindicator, status_message);
		sleep(updrate);
	}
#endif
	return NULL;
}
