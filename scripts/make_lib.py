import platform
import shutil
import subprocess
from pathlib import Path


ROOT_DIR = Path(__file__).parent.parent.absolute()
BUILD_DIR = ROOT_DIR.joinpath("build")


def make_windows() -> None:
	subprocess.check_output("bazel build DLL --config=DLL")
	output_dir = BUILD_DIR.joinpath("OrichalcumLib-windows-x86_64")
	if output_dir.exists():
		shutil.rmtree(output_dir)
	output_dir.mkdir()
	shutil.copy(
		ROOT_DIR.joinpath("bazel-bin/OrichalcumLib.dll"),
		output_dir.joinpath("OrichalcumLib.dll")
	)
	recurse_copy_headers(
		ROOT_DIR.joinpath("src/lib"),
		output_dir.joinpath("include")
	)
	shutil.make_archive(str(output_dir), "zip", output_dir)


def make_linux() -> None:
	raise NotImplemented


def recurse_copy_headers(src: Path, dest: Path) -> None:
	dest.mkdir()
	for item in src.iterdir():
		if item.is_dir():
			recurse_copy_headers(item, dest.joinpath(item.name))
		else:
			if item.name.endswith(".hpp"):
				shutil.copy(item, dest)


def main() -> None:
	if not BUILD_DIR.exists():
		BUILD_DIR.mkdir()
	subprocess.check_output("bazel clean")
	if platform.system() == "Windows":
		make_windows()
	elif platform.system() == "Linux":
		make_linux()

if __name__ == "__main__":
	main()
