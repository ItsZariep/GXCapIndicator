void cancelconfig(void)
{
	gtk_widget_destroy(dialog);
}

void on_preferences(GtkWidget *button, gpointer data)
{
	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
		gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER);
		gtk_window_set_title(GTK_WINDOW(dialog), "GXCapIndicator Settings");

		theme = gtk_icon_theme_get_default();
		info = gtk_icon_theme_lookup_icon(theme, "keyboard-caps-enabled", 48, 0);
		if (info != NULL)
		{
			icon = gtk_icon_info_load_icon(info, NULL);
			gtk_window_set_icon(GTK_WINDOW(dialog), icon);
			g_object_unref(icon);
			g_object_unref(info);
		}

		gtk_container_set_border_width(GTK_CONTAINER(dialog), 5);

		grid = gtk_grid_new();
		gtk_container_add(GTK_CONTAINER(dialog), grid);
		gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
		gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

			gshowcap = gtk_check_button_new();
			gshownum = gtk_check_button_new();
			gcapstate = gtk_check_button_new();
			gnumstate = gtk_check_button_new();
			gupdrate = gtk_entry_new();

			btn_cancel = gtk_button_new_with_label("cancel");
			btn_savesettings = gtk_button_new_with_label("save");

			gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show Caps Lock:"), 0, 0, 1, 1);
				gtk_grid_attach(GTK_GRID(grid), gshowcap, 1, 0, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Show Num Lock:"), 0, 1, 1, 1);
				gtk_grid_attach(GTK_GRID(grid), gshownum, 1, 1, 1, 1);
			#ifdef WITHX11
			gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Update rate (seconds):"), 0, 2, 1, 1);
				gtk_grid_attach(GTK_GRID(grid), gupdrate, 1, 2, 1, 1);
			#else
			gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Start with caps lock \n visually enabled:"), 0, 2, 1, 1);
				gtk_grid_attach(GTK_GRID(grid), gcapstate, 1, 2, 1, 1);
			gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Start with num lock \n visually enabled:"), 0, 3, 1, 1);
				gtk_grid_attach(GTK_GRID(grid), gnumstate, 1, 3, 1, 1);
			#endif
			gtk_grid_attach(GTK_GRID(grid), btn_cancel, 0, 6, 1, 1);
				gtk_grid_attach(GTK_GRID(grid), btn_savesettings, 1, 6, 1, 1);

		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gshowcap), !!showcap);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gshownum), !!shownum);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gcapstate), !!vcapstate);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gnumstate), !!vnumstate);

		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%d", updrate);
		gtk_entry_set_text(GTK_ENTRY(gupdrate), buffer);

		g_signal_connect(btn_cancel, "clicked", G_CALLBACK(cancelconfig), NULL);
		g_signal_connect(btn_savesettings, "clicked", G_CALLBACK(saveconfig), dialog);
	gtk_widget_show_all(dialog);
	gtk_main();
}
