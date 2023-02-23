# Weasel
A process manager library for linux with a qt frontend.

# Features
- Process listing in tree format
- Cpu and RAM load bars
- View network connections and network TX/RX rates
- View open files and disk R/W rates
- All Multiple-byte units are displayed in KB/GB/TB as opposed to KIB/GIB/TIB

# Planned Features
- User configurable coloring of processes
- User configurable process tree columns
- Search processes
- Sorting based on columns
- Load tab that shows graphs of the system resource usages
- Show icons for processes
- Window Manager interface
- Allow changing of process priorities
- Allow configuration of refresh interval

## Screenshots

# Permissions
Running Gui applications as a privileged user (Eg. <code>sudo</code>) is not advised because Gui libraries contain a lot of code which increases the likelihood of possibly exploitable bugs in the library code.

Weasel should be run as an unprivileged user, in which case it can still access the process tree and total system usage without elevated privileges. Details of processes running under a different user might not be available as an unprivileged user.

## Building

See [BUILDING.md](BUILDING.md)