#!/usr/bin/python3

# From: https://github.com/platformio/platform-espressif32/issues/1078#issuecomment-2219671743

# Adds PlatformIO post-processing to merge all the ESP flash images into a single image.

import os

Import("env", "projenv")

board_config = env.BoardConfig()
firmware_bin = "${BUILD_DIR}/${PROGNAME}.bin"
merged_bin = os.environ.get("MERGED_BIN_PATH", "${BUILD_DIR}/${PROGNAME}-merged.bin")


def merge_bin_action(source, target, env):
    flash_images = [
        *env.Flatten(env.get("FLASH_EXTRA_IMAGES", [])),
        "$ESP32_APP_OFFSET",
        source[0].get_abspath(),
    ]
    merge_cmd = " ".join(
        [
            '"$PYTHONEXE"',
            '"$OBJCOPY"',
            "--chip",
            board_config.get("build.mcu", "esp32"),
            "merge_bin",
            "-o",
            merged_bin,
            # ----------------------------------------------------------------
            # flash_mode / flash_freq 必须用 keep:
            # esptool >=4.8 会在 merge_bin 时改写 bootloader 头部的 flash
            # params 并重算 SHA。如果传具体值，bootloader 实际编译值与改写值
            # 不一致就会启动失败 (实测 ESP32-C6 board JSON 声明 qio,
            # 但 bootloader 实际按 dio 编译，强写 qio 后启动卡死)。
            # 4.5.x 因有 SHA 保护检测会忽略改写并打 warning, 行为等价于 keep。
            # ----------------------------------------------------------------
            "--flash_mode",
            "keep",
            "--flash_freq",
            "keep",
            "--flash_size",
            board_config.get("upload.flash_size", "4MB"),
            *flash_images,
        ]
    )
    env.Execute(merge_cmd)


env.AddCustomTarget(
    name="mergebin",
    dependencies=firmware_bin,
    actions=merge_bin_action,
    title="Merge binary",
    description="Build combined image",
    always_build=True,
)
