// Restricted file access functions for libinput
 int open_restricted(const char *path, int flags, void *user_data)
{
	int fd = open(path, flags);
	if (fd < 0)
	{
		perror("Failed to open restricted path");
	}
	return fd < 0 ? -errno : fd;
}

 void close_restricted(int fd, void *user_data)
{
	close(fd);
}

 const struct libinput_interface interface =
{
	.open_restricted = open_restricted,
	.close_restricted = close_restricted,
};

void check_udev(void)
{
	struct udev *udev = udev_new();
		if (udev == NULL)
		{
			g_error("Failed to initialize udev.");
			return;
		}

		libinput = libinput_udev_create_context(&interface, NULL, udev);
		if (!libinput)
		{
			g_error("Failed to initialize libinput from udev.");
			udev_unref(udev);
			return;
		}

		if (libinput_udev_assign_seat(libinput, "seat0") != 0)
		{
			g_error("Failed to set seat.");
			libinput_unref(libinput);
			udev_unref(udev);
			return;
		}
}

gboolean check_permissions(const gchar* group_name)
{
	gid_t *groups;
	int ngroups;
	struct group *grp;
	gid_t gid;

	ngroups = getgroups(0, NULL);
	groups = g_malloc(ngroups * sizeof(gid_t));
	getgroups(ngroups, groups);

	grp = getgrnam(group_name);
	if (!grp) {
		g_free(groups);
		return FALSE;
	}
	gid = grp->gr_gid;

	for (int i = 0; i < ngroups; i++)
	{
		if (groups[i] == gid)	
		{
			g_free(groups);
			return TRUE;
		}
	}

	g_free(groups);
	g_error("User is likely not in input group, please add your user to input group or compile this program with -DWITHX11 if you're running X11");
	return FALSE;
}
