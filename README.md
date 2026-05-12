# GXCapIndicator

<img src="https://codeberg.org/ItsZariep/pages-hugo/raw/branch/main/static/assets/projects/qxcapindicator/qxcapindicator.webp" alt="GXCapIndicator Logo" width="111px" />

> [!WARNING]
> ## Project migrated to Qt6 and Moved to [codeberg](https://codeberg.org/itszariep/qxcapindicator)
> [![Get in on Codeberg](https://codeberg.org/ItsZariep/.profile/raw/branch/main/movedtocodeberg.webp)](https://codeberg.org/itszariep/qxcapindicator)

 > [!WARNING]  
> due to [Ayatana](https://github.com/AyatanaIndicators/libayatana-appindicator-glib) core codebase being changed, this project may not work in the future, please use [QXCapIndicator](https://codeberg.org/itszariep/qxcapindicator) (Qt6 version of this program) if that happens


**GXCapIndicator** is a simple tool for indicating the status of Caps Lock and Num Lock keys in the system tray.

## Features

- [x] Monitor Caps Lock and Num Lock keys
- [x] Toggle Caps Lock and Num Lock with an on-screen button
- [x] Hideable indicators
- [x] Adjustable update rate
- [x] Wayland support (with `Evdev`)

## Installation

To build GXCapIndicator, choose the backend you want to use:

### With Evdev Backend

```ini
make
```

### With X11 Backend

```ini
make WITHX11=1
```

## Backend Differences

GXCapIndicator supports two backends for fetching key status: X11 and Evdev. Each backend has its own capabilities and limitations.

### X11 Backend
- Uses `XkbGetState` to fetch the key status.
- Checks cannot be done in real time without generating high CPU usage.
- Some options are not available:
  - **Initial Visual State**: Cannot set the initial visual state of Caps Lock/Num Lock (unnecessary with XkbGetState).
  
### Evdev Backend
- Uses `libinput_event_get_keyboard_event` to fetch the key status.
- Checks can be real time without high CPU Usage
- Some options are not available:
  - **Update Rate**: Cannot change update rate because polling intercepts events (and is not really necessary because there is no CPU usage penalty).
  - **Toggle**: Can't be implemented without root access.

## Configuration

You can configure GXCapIndicator with the integrated GUI (`settings` in indicator menu) or by editing` $HOME/.config/gxcapindicator` using the following options:

| Item        | Description                          | Possible Values | Default Value | Example     | Scope   |
|-------------|--------------------------------------|-----------------|---------------|-------------|---------|
| `showcap`   | Enables the Caps Lock key indicator  | `0`, `1`        | `1`           | `showcap=1` | All     |
| `shownum`   | Enables the Num Lock key indicator   | `0`, `1`        | `1`           | `shownum=1` | All     |
| `updrate`   | Sets the update rate                 | (Number)        | `0`           | `updrate=0` | X11     |
| `vcapstate` | Sets the initial visual state of Caps Lock | `0`, `1`   | `0`           | `vcapstate=0` | Evdev   |
| `vnumstate` | Sets the initial visual state of Num Lock  | `0`, `1`   | `0`           | `vnumstate=0` | Evdev   |
