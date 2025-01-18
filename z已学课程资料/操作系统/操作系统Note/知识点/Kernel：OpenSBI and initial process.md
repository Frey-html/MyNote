[[Kernel：BootStrap Program]]
### 1. **OpenSBI**:
**OpenSBI (Open Supervisor Binary Interface)** is a platform-specific software layer used primarily in **RISC-V** architecture. It serves as a foundational part of the boot process for RISC-V systems, acting like a minimal "firmware" or "bootloader" that allows the operating system (in this case, the Linux kernel) to interact with hardware features of the platform.

#### Role of OpenSBI:
- **RISC-V Privilege Levels**: RISC-V architecture defines multiple privilege levels, including:
  - **M-mode** (Machine mode) — the highest privilege level, usually where the firmware operates.
  - **S-mode** (Supervisor mode) — where the Linux kernel runs.
  - **U-mode** (User mode) — where user-space applications run.

  OpenSBI acts as the layer between the Machine mode (M-mode) and Supervisor mode (S-mode). It provides a set of **supervisor binary interface (SBI) functions** that the operating system, running in S-mode, can call to interact with the hardware.

- **Functionality**:
  - OpenSBI provides low-level services like:
    - Power management.
    - Handling hardware interrupts.
    - Booting into the operating system (i.e., transferring control to the Linux kernel).
    - Providing supervisor-mode runtime services for the kernel to interact with hardware (e.g., managing I/O devices, timers).
  
- **Boot Process with OpenSBI**:
  - During boot on RISC-V systems, OpenSBI is responsible for initializing the hardware, setting up the environment, and then loading the Linux kernel.
  - It operates in M-mode initially, and once the Linux kernel is started, OpenSBI hands off control to the kernel (which runs in S-mode).
  
In RISC-V systems, you often see OpenSBI messages during the early part of the boot process, as it's responsible for preparing the environment before the kernel fully takes over.

### 2. Diff with bootloader
No, **OpenSBI** is **not a bootloader** in the traditional sense like **GRUB** or **U-Boot**. It serves a different purpose in the **RISC-V** ecosystem, specifically providing an interface between the operating system and the hardware.
#### What is **OpenSBI**?

- **OpenSBI** (Open Supervisor Binary Interface) is an open-source implementation of the **SBI** (Supervisor Binary Interface) specification for **RISC-V** platforms.
- It provides the necessary software infrastructure to run **RISC-V Linux** or other operating systems in supervisor mode by managing machine mode resources (e.g., hardware access, timers, power management).
  
#### Role of OpenSBI in the Boot Process

In the **RISC-V** architecture, there are multiple privilege levels, with **machine mode (M-mode)** having the highest privileges, and **supervisor mode (S-mode)** being the mode in which the operating system (like Linux) typically runs. OpenSBI works in M-mode and provides services to the OS kernel running in S-mode.

##### Key functions of OpenSBI:
1. **Hardware Abstraction**: OpenSBI abstracts hardware details for the OS by offering services such as memory management, power control, and I/O operations.
2. **Bootstrapping the Kernel**: OpenSBI helps transition from machine mode (M-mode) to supervisor mode (S-mode) after the bootloader loads the kernel.
3. **System Calls for Supervisor Mode**: It provides standard functions for the kernel to interact with the hardware via SBI calls.

##### OpenSBI vs. Bootloaders (like GRUB or U-Boot)

| **Aspect**               | **OpenSBI**                                     | **GRUB/U-Boot (Bootloader)**                               |
|--------------------------|------------------------------------------------|------------------------------------------------------------|
| **Purpose**               | Provides an interface between the OS and hardware in RISC-V systems (SBI implementation) | Loads the OS kernel from disk into memory and passes control to it |
| **Mode of Operation**     | Runs in **machine mode** (M-mode) on RISC-V hardware | Runs in the early boot phase, loaded by firmware (BIOS/UEFI) |
| **Platform**              | Specific to **RISC-V** platforms               | Used on many platforms, including x 86, ARM, and RISC-V      |
| **Kernel Interaction**    | Provides runtime services for the kernel to interact with hardware | Bootstraps the kernel by loading it and passing control to it |
| **Stage in Boot Process** | Comes into play after the bootloader loads the kernel (before the OS runs in S-mode) | Operates before the OS kernel is loaded (early boot process) |

#### Example Boot Process Involving OpenSBI

In a typical RISC-V system using OpenSBI, the boot process looks something like this:

1. **Power-On**: The system firmware (BIOS/UEFI or similar) initializes hardware.
2. **First Stage Bootloader (FSBL)**: A minimal bootloader (like U-Boot or BBL for RISC-V) loads the OS kernel and OpenSBI binary.
3. **OpenSBI (M-mode)**: OpenSBI runs in machine mode, setting up the environment for the OS, and providing SBI services to the kernel.
4. **OS Kernel (S-mode)**: The Linux kernel (or another OS) runs in supervisor mode, relying on OpenSBI to handle low-level hardware interactions.

#### Summary:

- **OpenSBI** is **not** a bootloader but rather an interface layer that works with the **RISC-V** kernel to provide low-level services.
- Bootloaders like **GRUB** or **U-Boot** are responsible for loading the kernel into memory, whereas **OpenSBI** provides system services **after** the bootloader has handed off control to the kernel.


### 3. **/sbin/init**:
**`/sbin/init`** is the first process started by the Linux kernel once it has completed its initialization during boot. It is the **initialization process** that sets up the entire user space, starting all other processes and services on a Linux system. It is always the process with **PID 1**, which is the parent of all other processes.

#### Role of `/sbin/init`:
- **Init Process**:
  - After the kernel has booted and initialized the hardware, it will execute the `init` process (located at `/sbin/init` by default).
  - `init` is responsible for setting up the user-space environment, starting necessary system services, mounting filesystems, and spawning user-level processes.

- **System Initialization**:
  - `init` reads the configuration files (typically `/etc/inittab` or system-specific initialization scripts) and starts the system services (like networking, logging, etc.).
  - It runs all initialization scripts, sets up daemons, and prepares the system for user interaction (such as launching the login prompt).
  
- **Modern Linux Systems**:
  - On modern Linux distributions, `/sbin/init` often points to a more complex init system like:
    - **systemd** (common in most Linux distros).
    - **SysVinit** (traditional init system).
    - **Upstart** (used in older Ubuntu versions).
  
  If the system uses `systemd`, `/sbin/init` is typically a symlink to `systemd`, which then manages services, processes, and the overall state of the system.

#### Boot Process with `/sbin/init`:
- After the kernel has completed its own initialization, it executes `/sbin/init`:
  1. **Kernel boots and initializes** the hardware and essential kernel subsystems.
  2. **Kernel launches `/sbin/init`** as the first user-space process (PID 1).
  3. `init` reads its configuration files, **spawns system services**, and manages the overall system state (like booting into multi-user mode, starting daemons, etc.).
  4. It stays running in the background as the **parent of all other processes** and will eventually handle system shutdown or reboot.

#### Summary of Their Roles in Boot Process:
- **OpenSBI**:
  - Used in **RISC-V systems** to provide a low-level interface between the hardware (M-mode) and the Linux kernel (S-mode).
  - Initializes hardware and transfers control to the Linux kernel.
  - Similar to a minimal firmware or bootloader.
  
- **/sbin/init**:
  - The **first user-space process** that the kernel runs after completing its initialization.
  - Responsible for starting system services, daemons, and setting up the environment for users.
  - Parent process for all user-space processes (PID 1), and remains running throughout the lifetime of the system.

Together, **OpenSBI** is crucial for getting the Linux kernel running on RISC-V hardware, and **/sbin/init** is responsible for launching and managing all user-space processes once the kernel is up and running.