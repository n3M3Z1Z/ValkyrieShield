# ValkyrieShield

ValkyrieShield is a lightweight daemon designed to enhance the security of your system by monitoring file integrity and providing real-time alerts for unauthorized changes.

## Features

- **File Integrity Monitoring:** Continuously checks specified system files for unauthorized changes.
- **Kernel Integrity Checks:** Monitors kernel modules for modifications.
- **System File Monitoring:** Alerts on changes to critical system files.
- **Custom Script Execution:** Runs custom scripts on detecting anomalies.
- **Log Rotation and Management:** Efficiently manages logs to track changes.
- **Temporary File Creation:** Generates temporary analysis files that expire automatically.

## Installation

To install ValkyrieShield, start by cloning the repository:

```
git clone https://github.com/n3m3z1z/ValkyrieShield.git
cd ValkyrieShield
```

Compile the program with the following command:

```
gcc ValkyrieShield.c -o ValkyrieShield
```

After compilation, move the executable to a system-wide location:

```
sudo mv ValkyrieShield /usr/local/bin/
```

Set up the Systemd service by copying the service file:

```
sudo cp ValkyrieShield.service /etc/systemd/system/
```

Finally, enable and start the service:

```
sudo systemctl enable ValkyrieShield.service
sudo systemctl start ValkyrieShield.service
```

## Usage

To check the status of the ValkyrieShield daemon, use:

```
sudo systemctl status ValkyrieShield.service
```

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Author

**n3m3z1z** - [GitHub Profile](https://github.com/n3m3z1z)
