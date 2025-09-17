import os
import signal
import psutil

def kill_all_processes():
    current_pid = os.getpid()
    for proc in psutil.process_iter(['pid']):
        pid = proc.info['pid']
        # Skip current process and system critical processes (PID 1 is usually init/systemd)
        if pid != current_pid and pid != 1:
            try:
                os.kill(pid, signal.SIGKILL)
                print(f"Killed process {pid}")
            except PermissionError:
                print(f"Permission denied to kill process {pid}")
            except ProcessLookupError:
                print(f"Process {pid} does not exist")
            except Exception as e:
                print(f"Failed to kill process {pid}: {e}")

if __name__ == "__main__":
    kill_all_processes()

