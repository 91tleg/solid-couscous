# ...existing code...
import subprocess
import sys
import pathlib
import os

def run_test(binary: pathlib.Path) -> int:
    print(f"\n[RUNNING] {binary}")
    try:
        result = subprocess.run(
            [str(binary)],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            check=False
        )
        print(result.stdout)
        if result.stderr:
            print(result.stderr, file=sys.stderr)
        if result.returncode != 0:
            print(f"[FAILED] {binary} (exit code {result.returncode})")
        else:
            print(f"[PASSED] {binary}")
        return result.returncode
    except FileNotFoundError:
        print("[ERROR] Binary not found")
        return 1

def main():
    build_dir = pathlib.Path(__file__).parent / "build"
    if not build_dir.exists():
        print("Build directory not found")
        sys.exit(1)

    runners = sorted([p for p in build_dir.glob("*_runner") if p.is_file() and os.access(p, os.X_OK)])
    if runners:
        binaries = runners
    else:
        binaries = sorted([p for p in build_dir.glob("test_*") if p.is_file() and os.access(p, os.X_OK)])

    if not binaries:
        print("[ERROR] Binary not found")
        sys.exit(1)

    failures = 0
    for binary in binaries:
        failures += run_test(binary)

    if failures > 0:
        print(f"\n[SUMMARY] {failures} test(s) failed.")
        sys.exit(1)
    else:
        print("\n[SUMMARY] All tests passed.")
        sys.exit(0)

if __name__ == "__main__":
    main()