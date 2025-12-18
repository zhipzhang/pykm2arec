from pathlib import Path as _Path

from ._pykm2arec import *

# 自动设置配置文件路径为包安装目录下的 config 文件夹
_package_dir = _Path(__file__).parent
_config_path = _package_dir / "config"

if _config_path.exists():
    set_config_path(str(_config_path))