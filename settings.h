void create_num_indicator(void);
void create_cap_indicator(void);

void readconf(void)
{
	const gchar *config_dir = g_get_user_config_dir(); ;
	if (config_dir == NULL)
	{
		g_warning("Could not get g_get_user_config_dir");
		nohome = 1;
		return;
	}

	snprintf(config_file_path, sizeof(config_file_path), "%s/gxcapindicator.conf", config_dir);

	GKeyFile *keyfile = g_key_file_new();
	GError *error = NULL;

	if (!g_key_file_load_from_file(keyfile, config_file_path, G_KEY_FILE_NONE, &error))
	{
		if (error != NULL)
		{
			g_warning("Failed to load config file: %s", error->message);
			g_error_free(error);
		}
		nohome = 1;
		g_key_file_free(keyfile);
		return;
	}

	// Read values from the INI file
	showcap = g_key_file_get_integer(keyfile, "GXCapIndicator Configuration File", "showcap", NULL);
	shownum = g_key_file_get_integer(keyfile, "GXCapIndicator Configuration File", "shownum", NULL);
	updrate = g_key_file_get_integer(keyfile, "GXCapIndicator Configuration File", "updrate", NULL);
	vcapstate = g_key_file_get_integer(keyfile, "GXCapIndicator Configuration File", "vcapstate", NULL);
	vnumstate = g_key_file_get_integer(keyfile, "GXCapIndicator Configuration File", "vnumstate", NULL);

	g_key_file_free(keyfile);
}


void reset(GtkWidget *widget)
{
	readconf();
	app_indicator_set_status(capindicator, showcap ? APP_INDICATOR_STATUS_ACTIVE : APP_INDICATOR_STATUS_PASSIVE);
	app_indicator_set_status(numindicator, shownum ? APP_INDICATOR_STATUS_ACTIVE : APP_INDICATOR_STATUS_PASSIVE);

	if (showcap) 
	{
		create_cap_indicator();
		cleanup_thread(&cap_threadid, &cap_thread_active);
	}

	if (shownum) 
	{
		create_num_indicator();
		cleanup_thread(&num_threadid, &num_thread_active);
	}
}


void saveconfig(void)
{
	const gchar *cshowcap = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gshowcap)) ? "1" : "0";
	const gchar *cshownum = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gshownum)) ? "1" : "0";
	const gchar *cupdrate = gtk_entry_get_text(GTK_ENTRY(gupdrate));
	const gchar *cvcapstate = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gcapstate)) ? "1" : "0";
	const gchar *cvnumstate = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gnumstate)) ? "1" : "0";

	if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gshowcap)) && !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gshownum)))
	{
		dialog = gtk_message_dialog_new(GTK_WINDOW(window),
			GTK_DIALOG_MODAL,
			GTK_MESSAGE_ERROR,
			GTK_BUTTONS_OK,
			"ERROR: Invalid Situation\n\nIs not possible to have both 'shownum' and 'showcap' as FALSE");
		gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);

		info = gtk_icon_theme_lookup_icon(theme, "dialog-error", 48, 0);
		if (info != NULL)
		{
			icon = gtk_icon_info_load_icon(info, NULL);
			gtk_window_set_icon(GTK_WINDOW(dialog), icon);
			g_object_unref(icon);
			g_object_unref(info);
		}
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
		return;
	}

	GKeyFile *keyfile = g_key_file_new();
	GError *error = NULL;

	g_key_file_set_integer(keyfile, "GXCapIndicator Configuration File", "showcap", atoi(cshowcap));
	g_key_file_set_integer(keyfile, "GXCapIndicator Configuration File", "shownum", atoi(cshownum));
	g_key_file_set_integer(keyfile, "GXCapIndicator Configuration File", "vcapstate", atoi(cvcapstate));
	g_key_file_set_integer(keyfile, "GXCapIndicator Configuration File", "vnumstate", atoi(cvnumstate));
	g_key_file_set_string(keyfile, "GXCapIndicator Configuration File", "updrate", strlen(cupdrate) > 0 ? cupdrate : "1");

	if (!g_key_file_save_to_file(keyfile, config_file_path, &error))
	{
		g_warning("Failed to save config file: %s", error->message);
		g_error_free(error);
		g_key_file_free(keyfile);
		return;
	}

	g_key_file_free(keyfile);

	reset(window);
	gtk_widget_destroy(GTK_WIDGET(dialog));
}
