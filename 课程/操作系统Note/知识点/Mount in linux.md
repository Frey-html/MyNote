In Linux, **mounting** is the process of making a filesystem accessible to the operating system by attaching it to the directory tree. When you mount a device (like a hard drive, USB stick, or network storage), you assign it to a specific directory (called a **mount point**) in the Linux directory structure. Once mounted, the files and directories on that device are accessible from the mount point.

### Concepts about files
[[Device files, image and Blocks]]

### Key Concepts of Mounting

1. **Filesystem**:
   - A filesystem is a data structure that organizes files and directories on a storage device.
   - Examples: `ext4`, `NTFS`, `FAT32`, `XFS`, etc.

2. **Mount Point**:
   - This is the directory where the filesystem will be attached.
   - It can be any existing directory on the system (e.g., `/mnt`, `/media`, `/home`, or even a custom directory like `/mydrive`).
   - Once mounted, the contents of the filesystem (files, directories, etc.) will appear inside that directory.

3. **Unmounting**:
   - **Unmounting** is the opposite of mounting. It detaches the filesystem from the directory tree, making the files on the device no longer accessible from the mount point.
   - You must unmount a device before removing it (like a USB drive) to avoid data corruption.

### The `mount` Command

The `mount` command is used to manually mount a filesystem in Linux. The basic syntax is:

```bash
mount [OPTIONS] DEVICE MOUNT_POINT
```

- `DEVICE`: The storage device or partition you want to mount (e.g., `/dev/sda1`, `/dev/sdb1`, `/dev/nvme0n1p1`).
- `MOUNT_POINT`: The directory where the filesystem will be mounted (e.g., `/mnt`, `/media/usb`, `/home/user/external_drive`).

#### Example of Mounting a USB Drive:
Let’s say you have a USB drive that’s recognized as `/dev/sdb1` (the first partition on the second drive). To mount it at `/mnt/usb`:

1. First, create a mount point (if it doesn’t exist):
   ```bash
   sudo mkdir /mnt/usb
   ```

2. Mount the USB drive:
   ```bash
   sudo mount /dev/sdb1 /mnt/usb
   ```

3. Now, the contents of the USB drive will be accessible in the `/mnt/usb` directory.

#### Specifying the Filesystem Type:
Sometimes you need to specify the type of filesystem (e.g., `ext4`, `vfat`, `ntfs`), especially if the system doesn’t automatically detect it. You can do this with the `-t` option:

```bash
sudo mount -t ext4 /dev/sdb1 /mnt/usb
```

#### Common Options for `mount`:
- `-t`: Specifies the filesystem type (e.g., `ext4`, `vfat`, `ntfs`).
- `-o`: Specifies mount options (like `ro` for read-only, `rw` for read-write, etc.).
  - Example: Mount as read-only:
    ```bash
    sudo mount -o ro /dev/sdb1 /mnt/usb
    ```

---

### Automatically Mounting Filesystems: `/etc/fstab`

The `/etc/fstab` file is used to define filesystems that should be automatically mounted at boot. It lists the devices, mount points, and options for mounting them automatically.

An example `/etc/fstab` entry might look like this:

```
/dev/sdb1   /mnt/usb   ext4    defaults    0   2
```

- `/dev/sdb1`: The device to be mounted.
- `/mnt/usb`: The mount point.
- `ext4`: The filesystem type.
- `defaults`: Default mount options (read/write, noexec, etc.).
- `0`: Whether to dump (backup) the filesystem.
- `2`: The order in which filesystems are checked (root filesystem is usually `1`, others are `2`).

---

### Unmounting Filesystems: `umount`

To safely detach a filesystem, you use the `umount` command. The syntax is:

```bash
sudo umount MOUNT_POINT
```

Or

```bash
sudo umount DEVICE
```

For example, to unmount the USB drive we mounted earlier:

```bash
sudo umount /mnt/usb
```

Or

```bash
sudo umount /dev/sdb1
```

**Note**: Make sure no files or directories on the mounted filesystem are in use before unmounting, or the command will fail.

---

### Use Cases for Mounting

1. **Mounting External Drives**: When you connect an external drive (USB, external HDD, etc.), the system mounts it so you can access its files.
2. **Mounting Partitions**: You can mount additional partitions on your hard drive manually or automatically at boot.
3. **Network Filesystems**: You can mount remote filesystems (like NFS or CIFS/SMB) over the network.
   ```bash
   Sudo mount -t nfs 192.168.1.100:/shared /mnt/network
   ```

---

### Summary of `mount` in Linux

- **Mounting** is the process of attaching a filesystem to the directory structure, allowing access to its contents.
- The `mount` command is used to manually mount filesystems, while `/etc/fstab` controls automatic mounting at boot.
- Mounting makes the contents of storage devices available at a **mount point** (a directory), and unmounting detaches the device.

**Mounting** and **creating symbolic links** are both ways to access files and directories in a Linux system, but they serve different purposes and operate at different levels within the filesystem.

### Mounting and Symbol link

#### Mounting
**Mounting** is the process of attaching a **filesystem** (from a storage device, partition, or network share) to the existing directory structure of the operating system. When you mount a filesystem, you make its contents accessible through a specific directory (the mount point).
##### Key Points:
- **Filesystem Integration**: Mounting integrates an entire filesystem into the existing directory tree. The contents of the mounted filesystem appear as part of the directory structure.
- **Access to Devices**: Typically used for accessing partitions, external drives, network shares, or virtual filesystems.
- **Automatic and Manual**: Filesystems can be mounted manually using the `mount` command or automatically at boot via `/etc/fstab`.
- **Persistence**: The mount point is persistent while the filesystem is mounted. When the filesystem is unmounted, the directory at the mount point returns to its previous state.

**Example of Mounting**:
```bash
sudo mount /dev/sda1 /mnt/mydrive
```
- `/dev/sda1` is mounted to `/mnt/mydrive`. The contents of `/dev/sda1` are now accessible in `/mnt/mydrive`.

#### Symbolic Links

**Symbolic links** (or symlinks) are special types of files that point to other files or directories. They provide a way to create shortcuts or references to files or directories at different locations in the filesystem.

##### Key Points:
- **Reference to Files**: Symbolic links are pointers or shortcuts to another file or directory. They don’t actually contain the file data but reference the path to it.
- **File-Level Operation**: Creating a symlink doesn’t involve mounting a filesystem. It merely creates a new file that points to an existing file or directory.
- **Flexible Path Resolution**: Symbolic links can point to any path, including across different filesystems.
- **Temporary**: Symlinks are not persistent across different sessions or environments. They are just references and will break if the target file or directory is moved or deleted.

**Example of Creating a Symbolic Link**:
```bash
ln -s /path/to/original /path/to/symlink
```
- `/path/to/original` is the target file or directory.
- `/path/to/symlink` is the symbolic link that points to the target.

### Key Differences

1. **Scope**:
   - **Mounting** integrates an entire filesystem into the directory structure, making all its contents accessible.
   - **Symbolic Links** create a reference to a specific file or directory, allowing access through a shortcut.

2. **Operation Level**:
   - **Mounting** is an operation at the filesystem level, involving devices and partitions.
   - **Symbolic Links** operate at the file level, providing a shortcut to a specific file or directory.

3. **Persistence**:
   - **Mounting** is persistent as long as the filesystem remains mounted. The contents of the mount point are directly integrated into the directory structure.
   - **Symbolic Links** are persistent as long as the link exists and the target file or directory is available. If the target is moved or deleted, the symlink will be broken.

4. **Use Cases**:
   - **Mounting** is used when you need to integrate or access additional filesystems, partitions, or network shares.
   - **Symbolic Links** are used for creating shortcuts, organizing files, or linking to files or directories from different locations in the filesystem.

### Practical Example

- **Mounting**: If you have an external hard drive with a filesystem, you would mount it to access its contents. This allows the entire filesystem of the external drive to be accessible from a directory on your system.

- **Symbolic Link**: If you frequently need to access a specific directory from multiple locations, you might create a symlink to that directory. For example, you might create a symlink to a deep directory within your home directory to make it easier to access from the root of your home directory.

**Creating a Symlink to a Directory**:
```bash
ln -s /home/user/important_documents /home/user/shortcut_to_docs
```
- This creates a symlink named `shortcut_to_docs` in `/home/user`, which points to `/home/user/important_documents`.

**Mounting a Filesystem**:
```bash
sudo mount /dev/sdb1 /mnt/external
```
- This mounts the filesystem on `/dev/sdb1` to `/mnt/external`, making its contents accessible under `/mnt/external`.

By understanding the differences between mounting and symbolic links, you can use each method appropriately for different scenarios in managing files and directories in Linux.