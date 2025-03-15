void on_about(GtkMenuItem *menuitem, gpointer userdata)
{
	dialog = gtk_about_dialog_new();
	gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
		theme = gtk_icon_theme_get_default();
		info = gtk_icon_theme_lookup_icon(theme, "keyboard-caps-enabled", 48, 0);
		if (info != NULL)
		{
			icon = gtk_icon_info_load_icon(info, NULL);
			gtk_window_set_icon(GTK_WINDOW(dialog), icon);
			g_object_unref(icon);
			g_object_unref(info);
		}
	gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog), "GXCapIndicator");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog), "Copyright © 2024 ItzSelenux");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Simple Cap/Num lock indicator for X11 Tray");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), pver);
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://github.com/itzselenux/gxcapindicator");
	gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(dialog), "Project WebSite");
	gtk_about_dialog_set_license_type(GTK_ABOUT_DIALOG(dialog),GTK_LICENSE_GPL_3_0);
	gtk_about_dialog_set_logo_icon_name(GTK_ABOUT_DIALOG(dialog),"keyboard-caps-enabled");

	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

void create_cap_indicator(void)
{
	if (capvisible)
		return;
	capvisible = 1;

	capmenu = gtk_menu_new();
		capmenu_item_toggle = gtk_menu_item_new_with_label("Toggle Caps Lock");
		capmenu_item_settings = gtk_menu_item_new_with_label("Settings");
		capmenu_item_about = gtk_menu_item_new_with_label("About");
		capmenu_item_quit = gtk_menu_item_new_with_label("Quit");
		#ifdef WITHX11
		gtk_menu_shell_append(GTK_MENU_SHELL(capmenu), capmenu_item_toggle);
		#endif
		if (!nohome)
			gtk_menu_shell_append(GTK_MENU_SHELL(capmenu), capmenu_item_settings);
		gtk_menu_shell_append(GTK_MENU_SHELL(capmenu), capmenu_item_about);
		gtk_menu_shell_append(GTK_MENU_SHELL(capmenu), capmenu_item_quit);
		gtk_widget_show_all(capmenu);

	capindicator = app_indicator_new("gxcapindicator-capslock", "GXCapIndicator", APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
		app_indicator_set_menu(capindicator, GTK_MENU(capmenu));
		app_indicator_set_status(capindicator, APP_INDICATOR_STATUS_ACTIVE);
		#ifdef WITHX11
			app_indicator_set_icon_full(capindicator, "image-loading-symbolic", "Caps Lock");
		#else
			const gchar *icon_name = vcapstate ? "keyboard-caps-enabled" : "keyboard-caps-disabled";
			const gchar *status_message = vcapstate ? "Caps Lock: Enabled" : "Caps Lock: Disabled";
			app_indicator_set_icon_full(capindicator, icon_name, status_message);
			app_indicator_set_title(capindicator, status_message);
		#endif

	g_signal_connect(G_OBJECT(capmenu_item_toggle), "activate", G_CALLBACK(toggle_cap), NULL);
	g_signal_connect(G_OBJECT(capmenu_item_settings), "activate", G_CALLBACK(on_preferences), NULL);
	g_signal_connect(G_OBJECT(capmenu_item_about), "activate", G_CALLBACK(on_about), NULL);
	g_signal_connect(G_OBJECT(capmenu_item_quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
	if (pthread_create(&cap_threadid, NULL, check_cap, NULL) != 0)
	{
		g_error("Error creating thread.");
		return;
	}
}

void create_num_indicator(void)
{
	if (numvisible)
		return;
	numvisible = 1;
	nummenu = gtk_menu_new();
		nummenu_item_toggle = gtk_menu_item_new_with_label("Toggle Num Lock");
		nummenu_item_settings = gtk_menu_item_new_with_label("Settings");
		nummenu_item_about = gtk_menu_item_new_with_label("About");
		nummenu_item_quit = gtk_menu_item_new_with_label("Quit");
		#ifdef WITHX11
			gtk_menu_shell_append(GTK_MENU_SHELL(nummenu), nummenu_item_toggle);
		#endif
		if (!nohome)
			gtk_menu_shell_append(GTK_MENU_SHELL(nummenu), nummenu_item_settings);
		gtk_menu_shell_append(GTK_MENU_SHELL(nummenu), nummenu_item_about);
		gtk_menu_shell_append(GTK_MENU_SHELL(nummenu), nummenu_item_quit);
	gtk_widget_show_all(nummenu);
	numindicator = app_indicator_new("gxcapindicator-numlock","GXCapIndicator",APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
		app_indicator_set_menu(numindicator, GTK_MENU(nummenu));
		app_indicator_set_status(numindicator, APP_INDICATOR_STATUS_ACTIVE);
		#ifdef WITHX11
			app_indicator_set_icon_full(numindicator, "image-loading-symbolic", "Num Lock");
		#else
			const gchar *icon_name = vnumstate ? "keyboard-num-enabled" : "keyboard-num-disabled";
			const gchar *status_message = vnumstate ? "Num Lock: Enabled" : "Num Lock: Disabled";
			app_indicator_set_icon_full(numindicator, icon_name, status_message);
			app_indicator_set_title(numindicator, status_message);
		#endif
	g_signal_connect(G_OBJECT(nummenu_item_toggle), "activate", G_CALLBACK(toggle_num), NULL);
	g_signal_connect(G_OBJECT(nummenu_item_settings), "activate", G_CALLBACK(on_preferences), NULL);
	g_signal_connect(G_OBJECT(nummenu_item_about), "activate", G_CALLBACK(on_about), NULL);
	g_signal_connect(G_OBJECT(nummenu_item_quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
	if (pthread_create(&num_threadid, NULL, check_num, NULL) != 0)
	{
		g_error("Error creating thread.");
		return;
	}
}

void create_window(void)
{
	#ifdef WITHX11
	d = XOpenDisplay(NULL);
	if (d == NULL)
		g_error("can't open display");
	#endif

	if (showcap)
		create_cap_indicator();


	if (shownum)
		create_num_indicator();

	gtk_main();

	if (showcap)
	{
		pthread_cancel(cap_threadid);
		pthread_join(cap_threadid, NULL);
	}

	#ifdef WITHX11
	if (shownum)
	{
		pthread_cancel(num_threadid);
		pthread_join(num_threadid, NULL);
	}
	#endif
}
