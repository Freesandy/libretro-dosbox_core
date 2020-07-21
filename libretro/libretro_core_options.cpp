// This is copyrighted software. More information is at the end of this file.
#include "CoreOptions.h"
#include "libretro_dosbox.h"

// clang-format off

#define CYCLES_COARSE_MULTIPLIERS \
    100, \
    1000, \
    10000, \
    100000,

#define CYCLES_FINE_MULTIPLIERS \
    1, \
    10, \
    100, \
    1000, \
    10000,

#define CYCLES_VALUES 0, 1, 2, 3, 4, 5, 6, 7, 8, 9

#define MOUSE_SPEED_FACTORS \
    { \
        "0.10", "0.11", "0.12", "0.13", "0.14", "0.15", "0.16", "0.17", "0.18", "0.19", "0.20", \
        "0.21", "0.22", "0.23", "0.24", "0.25", "0.26", "0.27", "0.28", "0.29", "0.30", "0.31", \
        "0.32", "0.33", "0.34", "0.35", "0.36", "0.37", "0.38", "0.39", "0.40", "0.43", "0.45", \
        "0.48", "0.50", "0.55", "0.60", "0.65", "0.70", "0.75", "0.80", "0.85", "0.90", "0.95", \
        "1.00", "1.10", "1.17", "1.25", "1.38", "1.50", "1.63", "1.75", "2.00", "2.25", "2.50", \
        "2.75", "3.00", "3.25", "3.50", "3.75", "4.00", "4.25", "4.50", "4.75", "5.00", \
    }

retro::CoreOptions retro::core_options {
    "dosbox_core_",
    {
        {
            "use_options",
            "内核：启用内核设置（须重启）",
            "如果您想使用.conf文件指定设置，可禁用此处的内核设置。\n"
                "注意：您还可以和.conf文件一起配合使用此的处内核设置，\n"
                "例如只在.conf文件中包含[autoexec]选项，或者其他还无法在此处内核设置中更改的dosbox选项。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            true
        },
        {
            "adv_options",
            "内核：显示所有设置",
            "显示所有设置，包括那些通常不需要修改的设置。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            false
        },
        {
            "mount_c_as",
            "内核：将C盘挂载为",
            "当直接载入DOS可执行文件而不是.conf文件时，C盘可以挂载为可执行文件所在目录或者它的父目录。\n"
                "例如，当载入 DUKE3D 目录中的 DUKE3D.EXE 文件时，\n"
                "如果此选项设置为“游戏文件目录”，则DOS识别该文件为 C:\\DUKE3D.EXE，\n"
                "如果此选项设置为“游戏文件父目录”，则该文件识别为 C:\\DUKE3D\\DUKE3D.EXE。",
            {
                { "content", "游戏文件目录" },
                { "parent", "游戏文件父目录" },
            },
            "content"
        },
        {
            "default_mount_freesize",
            "内核：缺省挂载C盘的剩余空间",
            "该项是直接加载DOS可执行文件时C盘的剩余空间，\n"
                "等同于使用.conf文件加载时MOUNT指令的\"-freesize\"参数值。",
            {
                { 256, "256MB" },
                { 384, "384MB" },
                { 512, "512MB" },
                { 768, "768MB" },
                { 1024, "1GB" },
                { 1280, "1.25GB" },
                { 1536, "1.5GB" },
                { 1792, "1.75GB" },
            },
            1024
        },
        {
            "save_overlay",
            "内核：启用覆盖文件系统（须重启）",
            "启用覆盖文件系统可以重定向文件系统更改到存档目录。\n"
            "如果启动游戏时遇到问题可以尝试禁用此项。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            false
        },
        {
            "core_timing",
            "内核：时序模式",
            "外部模式是建议设置，此模式使前端可以控制帧数调配，\n"
            "没有输入延迟，并且允许前端功能比如动态帧率控制和帧延迟正常工作。\n"
            "内部模式和前端不同步运行，此模式允许以60FPS的输出运行70FPS的游戏，\n"
            "但是输入延迟和顿挫/抖动会增加。\n"
            "只有在你绝对需要以60FPS的输出运行70FPS游戏且不产生画面撕裂的情况下时使用此模式。",
            {
                { "external", "外部模式" },
                { "internal", "内部模式（固定60FPS）" },
            },
            "external"
        },
        {
            "thread_sync",
            "内核：线程同步方式",
            "“等待”是建议的方式，此方式在大多数系统上工作良好。\n"
                "如果因为某些原因此方式不能很好工作并且你能察觉到顿挫，\n"
                "设置为“轮询”方式可能会有帮助（也可能变得更糟）\n"
                "然而，“轮询”方式会使某个CPU核心占用率为100%，\n "
                "这是一种“空载”，并不会太大的增加CPU发热量，\n"
                "但是它会阻止CPU降频，在笔记本系统上会影响电池续航寿命。",
            {
                { "wait", "等待" },
                { "spin", "轮询" },
            },
            "wait"
        },
        {
            "machine_type",
            "系统：模拟显示硬件（须重启）",
            "DOSBox模拟的视频硬件类型。",
            {
                { "hercules", "大力神显示器" },
                { "cga", "CGA" },
                { "tandy", "Tandy" },
                { "pcjr", "PCjr" },
                { "ega", "EGA" },
                { "vgaonly", "VGA" },
                { "svga_s3", "SVGA (S3 Trio64)" },
                { "vesa_nolfb", "SVGA (S3 Trio64 无行缓冲破解)" },
                { "vesa_oldvbe", "SVGA (S3 Trio64 VESA 1.3)" },
                { "svga_et3000", "SVGA (Tseng Labs ET3000)" },
                { "svga_et4000", "SVGA (Tseng Labs ET4000)" },
                { "svga_paradise", "SVGA (Paradise PVGA1A)" },
            },
            "svga_s3"
        },
        {
            "machine_hercules_palette",
            "系统：大力神色彩模式",
            "大力神模拟的色彩方案。",
            {
                { 0, "黑色和白色" },
                { 1, "黑色和琥珀色" },
                { 2, "黑色和绿色" },
            },
            0
        },
        {
            "machine_cga_composite_mode",
            "系统：CGA复合模式切换",
            "启用或禁用CGA复合模式。",
            {
                { 0, "自动" },
                { 1, "启用" },
                { 2, "禁用" },
            },
            0
        },
        {
            "machine_cga_model",
            "系统：CGA型号",
            "模拟系统的CGA型号。",
            {
                { 0, "晚期型号" },
                { 1, "早期型号" },
            },
            0
        },
        #ifdef WITH_VOODOO
        {
            "voodoo",
            "系统：3dfx Voodoo模拟（须重启）",
            "模拟真实3dfx硬件，不是模拟glide API，因此不需要也不支持glide封装模拟。\n"
            "当前只支持慢速（非常慢）的软件模拟方式，绝大多数游戏还不能达到可玩的速度。",
            {
            // Not implemented yet.
            #if 0
                "opengl",
            #endif
                { "software", "软件模拟" },
                { false, "无" },
            },
            false
        },
        {
            "voodoo_memory_size",
            "系统：Voodoo卡显存大小（须重启）",
            "模拟Voodoo卡的显存大小。\n"
            "4MB是原始Voodoo卡的标准配置，12MB是非标准配置。",
            {
                { "standard", "4MB" },
                { "max", "12MB" },
            },
            "standard"
        },
        #endif
        {
            "memory_size",
            "系统：内存大小（须重启）",
            "模拟机器的系统内存大小。\n"
            "该项最好保持缺省值以避免部分游戏出现问题，但是少数游戏可能需要更大的内存。",
            {
                { 4, "4MB" },
                { 8, "8MB" },
                { 16, "16MB" },
                { 24, "24MB" },
                { 32, "32MB" },
                { 48, "48MB" },
                { 64, "64MB" },
            },
            16
        },
        {
            "cpu_core",
            "系统：CPU内核",
            "用于CPU模拟的内核。"
        #if defined(C_DYNREC) || defined(C_DYNAMIC_X86)
            "设为“自动”时，“正常”解释器内核将用于实模式游戏，\n"
            "而更快的“动态”重编译内核将用于保护模式游戏。\n"
            "“简单”解释器内核是为老的实模式游戏优化的内核。",
            {
                { "auto", "自动" },
            #if defined(C_DYNREC)
                { "dynamic", "动态重编译器（通用）" },
            #else
                { "dynamic", "动态重编译器（X86）" },
            #endif
                { "normal", "正常解释器" },
                { "simple", "简单解释器" },
            },
            "auto"
        #else
            "“简单”解释器内核是为老的实模式游戏优化的。\n"
            "（当前平台没有可用的动态重编译内核）",
            {
                { "normal", "正常解释器" },
                { "simple", "简单解释器" },
            },
            "normal"
        #endif
        },
        {
            "cpu_type",
            "系统：CPU类型",
            "模拟的CPU类型。“自动”是最快的选择。",
            {
                { "auto", "自动" },
                "386",
                { "386_slow", "386（低速）" },
                { "386_prefetch", "386（预取队列模拟） (prefetch queue emulation)" },
                "486",
                { "486_slow", "486（低速）" },
                { "pentium_slow", "奔腾（低速）" },
            },
            "auto"
        },
        {
            "cpu_cycles_mode",
            "系统：CPU时钟周期模式",
            "决定每毫秒模拟的CPU时钟周期数的方法。\n"
            "“固定”模式运行你设置的固定周期数。\n"
            "“最大”模式尽可能运行更多的周期数，依赖于设置的上限。可以同时设置一个最大CPU负载百分比和周期数作为限制。\n"
            "“自动”模式在运行实模式游戏时，运行在“实模式”周期数选项中设定的固定周期数；\n"
            "运行保护模式游戏时则切换到“最大”模式。\n"
            "“固定”模式和适当的周期数设置组合是兼容性最好的设置，因为“自动”和“最大”模式在很多系统上有问题。",
            {
                { "auto", "自动" },
                { "fixed", "固定" },
                { "max", "最大" },
            },
            "fixed"
        },
        {
            "cpu_cycles_multiplier_realmode",
            "系统: 实模式粗略CPU周期数倍率",
            "“自动”模式下运行实模式游戏时的CPU周期数粗略调整倍率。",
            { CYCLES_COARSE_MULTIPLIERS },
            1000
        },
        {
            "cpu_cycles_realmode",
            "系统：实模式粗略CPU周期基数",
            "“自动”模式下运行实模式游戏时的CPU周期数粗略调整基数。",
            { CYCLES_VALUES },
            3
        },
        {
            "cpu_cycles_multiplier_fine_realmode",
            "系统: 实模式精细CPU周期数倍率",
            "“自动”模式下运行实模式游戏时的CPU周期数精细调整倍率。",
            { CYCLES_FINE_MULTIPLIERS },
            100
        },
        {
            "cpu_cycles_fine_realmode",
            "系统: 实模式精细CPU周期基数",
            "“自动”模式下运行实模式游戏时的CPU周期数精细调整基数。",
            { CYCLES_VALUES },
            0
        },
        {
            "cpu_cycles_limit",
            "系统：最大CPU周期数限制",
            "“最大”模式的CPU周期数最大限制。",
            {
                { "none", "无限制" },
                "10%",
                "20%",
                "30%",
                "40%",
                "50%",
                "60%",
                "70%",
                "80%",
                "90%",
                "100%",
                "105%",
            },
            "100%"
        },
        {
            "cpu_cycles_multiplier",
            "系统：粗略CPU周期数倍数",
            "CPU周期数粗略调整倍率。",
            { CYCLES_COARSE_MULTIPLIERS },
            10000
        },
        {
            "cpu_cycles",
            "系统：粗略CPU周期基数",
            "CPU周期数粗略调整基数。",
            { CYCLES_VALUES },
            1
        },
        {
            "cpu_cycles_multiplier_fine",
            "系统：精细CPU周期数倍数",
            "CPU周期数精细调整倍率。",
            { CYCLES_FINE_MULTIPLIERS },
            1000
        },
        {
            "cpu_cycles_fine",
            "系统：精细CPU周期基数",
            "CPU周期数精细调整基数。",
            { CYCLES_VALUES },
            0
        },
        {
            "aspect_correction",
            "视频：宽高比校正",
            "启用时，宽高比会匹配对应的CRT显示器，以使非方形像素的VGA分辨率看上去和预期的一样。\n"
            "如果你期望无缩放的方形像素宽高比，可以禁用此项，但是可能会导致图像压扁或者拉长。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            true
        },
        {
            "scaler",
            "视频：DOSBox缩放器",
            "DOSBox内置的基于CPU运算的缩放器。此功能在这里提供以作为最后手段使用。\n"
            "你通常应该把此项设置为“无”，而使用前端程序提供的缩放功能和渲染器。",
            {
                "normal2x",
                "normal3x",
                "advmame2x",
                "advmame3x",
                "advinterp2x",
                "advinterp3x",
                "hq2x",
                "hq3x",
                "2xsai",
                "super2xsai",
                "supereagle",
                "tv2x",
                "tv3x",
                "rgb2x",
                "rgb3x",
                "scan2x",
                "scan3x",
                { "none", "无" },
            },
            "none"
        },
        {
            "joystick_force_2axis",
            "输入：强制两轴/两键",
            "正常情况下，当只连接了一个摇杆或手柄到时，缺省模拟4轴和4键，\n"
            "然而有些游戏（通常是较老的游戏）只有在经典的2轴/2键摇杆下才能正常工作。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            false
        },
        {
            "joystick_timed",
            "输入：启用摇杆定时间隔",
            "在摇杆轴上启用定时间隔。如果你的摇杆偏移，尝试一下该选项。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            false
        },
        {
            "emulated_mouse",
            "输入：启用手柄模拟鼠标",
            "使用手柄右摇杆模拟鼠标。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            false
        },
        {
            "emulated_mouse_deadzone",
            "输入：手柄模拟鼠标死区",
            "手柄模拟鼠标时的死区。如果鼠标光标漂移，尝试修改该选项。",
            {
                { 0, "0%" },
                { 5, "5%" },
                { 10, "10%" },
                { 15, "15%" },
                { 20, "20%" },
                { 25, "25%" },
                { 30, "30%" },
            },
            30
        },
        {
            "mouse_speed_factor_x",
            "输入：鼠标水平方向灵敏度",
            "如果鼠标左右移动速度太快，尝试修改此选项。",
            MOUSE_SPEED_FACTORS,
            "1.00"
        },
        {
            "mouse_speed_factor_y",
            "输入：鼠标垂直方向灵敏度",
            "如果鼠标上下移动速度太快，尝试修改此选项。",
            MOUSE_SPEED_FACTORS,
            "1.00"
        },
        {
            "sblaster_type",
            "声音：声霸卡型号",
            "模拟声霸卡的型号。",
            {
                { "sb1", "SoundBlaster 1.0" },
                { "sb2", "SoundBlaster 2.0" },
                { "sbpro1", "SoundBlaster Pro" },
                { "sbpro2", "SoundBlaster Pro 2" },
                { "sb16", "SoundBlaster 16" },
                { "gb", "GameBlaster" },
                { "none", "无" },
            },
            "sb16"
        },
        {
            "sblaster_base",
            "声音：声霸卡基地址",
            "模拟声霸卡的I/O地址。",
            {
                "220",
                "240",
                "260",
                "280",
                "2a0",
                "2c0",
                "2e0",
                "300",
            },
            "220"
        },
        {
            "sblaster_irq",
            "声音：声霸卡IRQ号",
            "模拟声霸卡的IRQ中断号。",
            {
                3,
                5,
                7,
                9,
                10,
                11,
                12,
            },
            7
        },
        {
            "sblaster_dma",
            "声音：声霸卡DMA通道",
            "模拟声霸卡的DMA通道号。",
            {
                0,
                1,
                3,
                5,
                6,
                7,
            },
            1
        },
        {
            "sblaster_hdma",
            "声音：声霸卡高位DMA通道",
            "模拟声霸卡的高位DMA通道号。",
            {
                0,
                1,
                3,
                5,
                6,
                7,
            },
            5
        },
        {
            "sblaster_opl_mode",
            "声音：声霸卡OPL模式",
            "模拟声霸卡的OPL模式。除CMS模式外其他模式都和Adlib兼容。",
            {
                { "auto", "自动（基于声霸卡型号自动选择）" },
                { "cms", "CMS (Creative Music System / GameBlaster)" },
                { "opl2", "OPL-2 (AdLib / OPL-2 / Yamaha 3812)" },
                { "dualopl2", "Dual OPL-2 (用于 SoundBlaster Pro 1.0 立体声输出)" },
                { "opl3", "OPL-3 (AdLib / OPL-3 / Yamaha YMF262)" },
                { "opl3gold", "OPL-3 Gold (AdLib Gold / OPL-3 / Yamaha YMF262)" },
                { "none", "无" },
            },
            "auto"
        },
        {
            "sblaster_opl_emu",
            "声音：声霸卡OPL驱动",
            "“Nuked OPL3”是精确时钟周期模拟的 OPL3 (YMF262) 模拟器，\n"
            "提供最好的音质，但是极度依吃CPU。\n"
            "“兼容模式”是第二好的选择，精确性稍低，但也较少吃CPU。",
            {
                { "nuked", "Nuked OPL3" },
                { "compat", "兼容模式" },
                { "mame", "MAME模式" },
                { "fast", "快速模式" },
            },
            "compat"
        },
        {
            "gus",
            "声音：Gravis Ultrasound支持",
            "启用Gravis Ultrasound模拟。ULTRADIR目录不可更改，固定为 C:\\ULTRASND",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            false
        },
        {
            "gusrate",
            "声音：Ultrasound采样率",
            "Gravis Ultrasound模拟采样率。",
            {
                8000,
                11025,
                16000,
                22050,
                32000,
                44100,
                48000,
                49716,
            },
            44100
        },
        {
            "gusbase",
            "声音：Ultrasound I/O地址",
            "模拟Gravis Ultrasound卡的I/O基地址。",
            {
                "220",
                "240",
                "260",
                "280",
                "2a0",
                "2c0",
                "2e0",
                "300",
            },
            "240"
        },
        {
            "gusirq",
            "声音：Ultrasound IRQ号",
            "模拟Gravis Ultrasound卡的IRQ中断号。",
            {
                3,
                5,
                7,
                9,
                10,
                11,
                12,
            },
            5
        },
        {
            "gusdma",
            "声音：Ultrasound DMA通道",
            "模拟Gravis Ultrasound卡的DMA通道号。",
            {
                0,
                1,
                3,
                5,
                6,
                7,
            },
            3
        },
        {
            "mpu_type",
            "声音：MPU-401类型",
            "模拟MPU-401 MIDI接口的类型。“智能选择”模式是最佳选择。",
            {
                { "intelligent", "智能选择" },
                { "uart", "UART" },
                "none",
            },
            "intelligent"
        },
        {
            "midi_driver",
            "声音：MIDI驱动",
            "MT-32模拟驱动使用Munt，需要把正确的ROM文件放到system目录下。\n"
            #ifdef WITH_BASSMIDI
            "对于BASSMIDI，需要从 https://www.un4seen.com\n"
            "下载和操作系统对应的BASS和BASSMIDI库，并且放到system目录下。\n"
            #endif
            "libretro驱动转发MIDI到前端程序，需要到RA设置里配置MIDI输出。",
            {
            #ifdef HAVE_ALSA
                { "alsa", "ALSA" },
            #endif
            #ifdef __WIN32__
                { "win32", "Windows MIDI" },
            #endif
            #ifdef WITH_BASSMIDI
                { "bassmidi", "BASSMIDI" },
            #endif
            #ifdef WITH_FLUIDSYNTH
                { "fluidsynth", "FluidSynth" },
            #endif
                { "mt32", "MT-32模拟器" },
                "libretro",
                { "none", "无" },
            },
            "none"
        },
        #ifdef HAVE_ALSA
        {
            "midi_port",
            "声音：ALSA MIDI端口",
            "发送MIDI到ALSA驱动的端口。"
            // No values. We detect and set MIDI ports at runtime.
        },
        #endif
        #ifdef __WIN32__
        {
            "midi_port",
            "声音：Windows MIDI端口",
            "发送MIDI到Windows驱动的端口。"
            // No values. We detect and set MIDI ports at runtime.
        },
        #endif
        #ifdef WITH_BASSMIDI
        {
            "bassmidi.soundfont",
            "声音 (BASSMIDI)：音色库",
            "音色库文件存放在system目录下的 \"soundfonts\" 目录中。\n"
            "支持SF2和SFZ格式。",
            // No values. We scan for soundfonts at runtime.
        },
        {
            "bassmidi.sfvolume",
            "声音 (BASSMIDI)：音色库音量",
            {
                "0.0",
                "0.1",
                "0.2",
                "0.3",
                "0.4",
                "0.5",
                "0.6",
                "0.7",
                "0.8",
                "0.9",
                "1.0",
                "1.1",
                "1.2",
                "1.3",
                "1.4",
                "1.5",
                "1.6",
                "1.7",
                "1.8",
                "1.9",
                "2.0",
                "2.5",
                "3.0",
                "3.5",
                "4.0",
                "4.5",
                "5.0",
                "6.0",
                "7.0",
                "8.0",
                "9.0",
                "10.0",
            },
            "0.6",
        },
        {
            "bassmidi.voices",
            "声音 (BASSMIDI)：发音数",
            "最大同时播放的乐器数。这不同于最大音符数，多个乐器可以同时演奏同一个音符。",
            {
                20,
                30,
                40,
                50,
                60,
                70,
                80,
                90,
                100,
                120,
                140,
                160,
                180,
                200,
                250,
                300,
                350,
                400,
                450,
                500,
                600,
                700,
                800,
                900,
                1000,
            },
            100
        },
        #endif
        #ifdef WITH_FLUIDSYNTH
        {
            "fluid.soundfont",
            "声音 (FluidSynth)：音色库",
            "音色库文件存放在system目录下的 \"soundfonts\" 目录中。\n"
            "支持SF2, SF3, DLS和GIG格式。推荐使用SF2和SF3格式。",
            // No values. We scan for soundfonts at runtime.
        },
        {
            "fluid.samplerate",
            "声音 (FluidSynth)：采样率",
            "合成器生成的音频采样率。",
            {
                { 8000, "8kHz" },
                { 11025, "11.025kHz" },
                { 16000, "16kHz" },
                { 22050, "22.05kHz" },
                { 32000, "32kHz" },
                { 44100, "44.1kHz" },
                { 48000, "48kHz" },
                { 96000, "96kHz" },
            },
            44100
        },
        {
            "fluid.gain",
            "声音 (FluidSynth)：音量增益",
            "音量增益应用于合成器的最终输出。\n"
            "此值通常应该较低 (0.2-0.5)，以避免引起音频削峰和失真。",
            {
                "0.0",
                "0.1",
                "0.2",
                "0.3",
                "0.4",
                "0.5",
                "0.6",
                "0.7",
                "0.8",
                "0.9",
                "1.0",
                "1.1",
                "1.2",
                "1.3",
                "1.4",
                "1.5",
                "1.6",
                "1.7",
                "1.8",
                "1.9",
                "2.0",
                "2.5",
                "3.0",
                "3.5",
                "4.0",
                "4.5",
                "5.0",
                "6.0",
                "7.0",
                "8.0",
                "9.0",
                "10.0",
            },
            "0.4",
        },
        {
            "fluid.polyphony",
            "声音 (FluidSynth)： 复音数",
            "复音数蒂尼可以同时播放的乐器数。值越高越耗CPU。",
            {
                1,
                2,
                4,
                8,
                16,
                32,
                64,
                128,
                256,
                384,
                512,
                768,
                1024,
                1536,
                2048,
                3072,
                4096,
            },
            256
        },
        {
            "fluid.cores",
            "声音 (FluidSynth)：CPU内核数",
            "设置合成器使用的CPU内核数。如果设置大于1，将创建附加的合成器线程以利用多核CPU系统。",
            {
                1,
                2,
                3,
                4,
                5,
                6,
                7,
                8,
                9,
                10,
                11,
                12,
                13,
                14,
                15,
                16,
                20,
                24,
                32
            },
            1
        },
        {
            "fluid.reverb",
            "声音 (FluidSynth)：启用混响",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            true
        },
        {
            "fluid.reverb.roomsize",
            "声音 (FluidSynth)：混响房间大小",
            {
                "0.0",
                "0.1",
                "0.2",
                "0.3",
                "0.4",
                "0.5",
                "0.6",
                "0.7",
                "0.8",
                "0.9",
                "1.0",
            },
            "0.2"
        },
        {
            "fluid.reverb.damping",
            "声音 (FluidSynth)：混响衰减率",
            {
                "0.0",
                "0.1",
                "0.2",
                "0.3",
                "0.4",
                "0.5",
                "0.6",
                "0.7",
                "0.8",
                "0.9",
                "1.0",
            },
            "0.0"
        },
        {
            "fluid.reverb.width",
            "声音 (FluidSynth)：混响宽度",
            {
                "0.0",
                "0.1",
                "0.2",
                "0.3",
                "0.4",
                "0.5",
                "0.6",
                "0.7",
                "0.8",
                "0.9",
                "1.1",
                "1.2",
                "1.3",
                "1.4",
                "1.5",
                "1.6",
                "1.7",
                "1.8",
                "1.9",
                "2.0",
                "2.5",
                "3.0",
                "3.5",
                "4.0",
                "4.5",
                "5.0",
                "6.0",
                "7.0",
                "8.0",
                "9.0",
                "10.0",
                "12.0",
                "14.0",
                "16.0",
                "18.0",
                "20.0",
                "25.0",
                "30.0",
                "35.0",
                "40.0",
                "45.0",
                "50.0",
                "60.0",
                "70.0",
                "80.0",
                "90.0",
                "100.0",
            },
            "0.5"
        },
        {
            "fluid.reverb.level",
            "声音 (FluidSynth)：混响级别",
            {
                "0.0",
                "0.1",
                "0.2",
                "0.3",
                "0.4",
                "0.5",
                "0.6",
                "0.7",
                "0.8",
                "0.9",
                "1.0",
            },
            "0.9"
        },
        {
            "fluid.chorus",
            "声音 (FluidSynth)：启用和声",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            true
        },
        {
            "fluid.chorus.number",
            "声音 (FluidSynth)：和声数",
            {
                0,
                1,
                2,
                3,
                4,
                5,
                6,
                7,
                8,
                9,
                10,
                11,
                12,
                14,
                16,
                18,
                20,
                24,
                28,
                32,
                56,
                64,
                96,
            },
            3
        },
        {
            "fluid.chorus.level",
            "声音 (FluidSynth)：和声级别",
            {
                "0.0",
                "0.2",
                "0.4",
                "0.6",
                "0.8",
                "1.0",
                "1.2",
                "1.4",
                "1.6",
                "1.8",
                "2.0",
                "2.2",
                "2.4",
                "2.6",
                "2.8",
                "3.0",
                "3.5",
                "4.0",
                "4.5",
                "5.0",
                "5.5",
                "6.0",
                "6.5",
                "7.0",
                "7.5",
                "8.0",
                "8.5",
                "9.0",
                "9.5",
                "10.0",
            },
            "2.0"
        },
        {
            "fluid.chorus.speed",
            "声音 (FluidSynth)：和声速度",
            {
                "0.1",
                "0.2",
                "0.3",
                "0.4",
                "0.5",
                "0.6",
                "0.7",
                "0.8",
                "0.9",
                "1.0",
                "1.1",
                "1.2",
                "1.3",
                "1.4",
                "1.5",
                "1.6",
                "1.7",
                "1.8",
                "1.9",
                "2.0",
                "2.1",
                "2.2",
                "2.3",
                "2.4",
                "2.5",
                "2.6",
                "2.7",
                "2.8",
                "2.9",
                "3.0",
                "3.1",
                "3.2",
                "3.3",
                "3.4",
                "3.5",
                "3.6",
                "3.7",
                "3.8",
                "3.9",
                "4.0",
                "4.1",
                "4.2",
                "4.3",
                "4.4",
                "4.5",
                "4.6",
                "4.7",
                "4.8",
                "4.9",
                "5.0",
            },
            "0.3"
        },
        {
            "fluid.chorus.depth",
            "声音 (FluidSynth)：和声深度",
            {
                0,
                1,
                2,
                3,
                4,
                5,
                6,
                7,
                8,
                9,
                10,
                11,
                12,
                13,
                14,
                15,
                16,
                18,
                20,
                22,
                24,
                28,
                32,
                48,
                64,
                80,
                96,
                112,
                128,
                160,
                192,
                224,
                256,
            },
            8
        },
        #endif
        {
            "mt32.type",
            "声音 (MT-32)：硬件型号",
            "模拟MT-32的型号。MT-32是老的原始型号；\n"
            "CM-32L和LAPC-I是后期型号，提供了一些老型号没有的额外的乐器音色库。\n"
            "某些使用这些额外音色的游戏在原始MT-32型号上不能正确发声。",
            {
                { "mt32", "MT-32" },
                { "cm32l", "CM-32L/LAPC-I" },
            },
            "cm32l"
        },
        {
            "mt32.reverse.stereo",
            "声音 (MT-32)：反转立体声声道",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            false
        },
        {
            "mt32.thread",
            "声音 (MT-32)：多线程模拟",
            "用独立线程运行MT-32模拟。在多核系统上可提高性能。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            false
        },
        {
            "mt32.chunk",
            "声音 (MT-32)：线程分块大小",
            "同时渲染的数据最小毫秒数。增加此值可减小渲染额外消耗，提升性能，但是也会增加音频延迟。",
            {
                { 2, "2ms" },
                { 3, "3ms" },
                { 4, "4ms" },
                { 5, "5ms" },
                { 7, "7ms" },
                { 10, "10ms" },
                { 13, "13ms" },
                { 16, "16ms" },
                { 20, "20ms" },
                { 24, "24ms" },
                { 28, "28ms" },
                { 32, "32ms" },
                { 40, "40ms" },
                { 48, "48ms" },
                { 56, "56ms" },
                { 64, "64ms" },
                { 80, "80ms" },
                { 96, "96ms" },
            },
            16
        },
        {
            "mt32.prebuffer",
            "声音 (MT-32)：线程预缓冲大小",
            "提前渲染的毫秒数。增加此值可有助于避免欠载运行，但是也会增加音频延迟。\n"
            "不能小于等于分块大小。",
            {
                { 3, "3ms" },
                { 4, "4ms" },
                { 5, "5ms" },
                { 7, "7ms" },
                { 10, "10ms" },
                { 13, "13ms" },
                { 16, "16ms" },
                { 20, "20ms" },
                { 24, "24ms" },
                { 28, "28ms" },
                { 32, "32ms" },
                { 40, "40ms" },
                { 48, "48ms" },
                { 56, "56ms" },
                { 64, "64ms" },
                { 80, "80ms" },
                { 96, "96ms" },
                { 112, "112ms" },
                { 128, "128ms" },
                { 144, "144ms" },
                { 160, "160ms" },
                { 176, "176ms" },
                { 192, "192ms" },
            },
            32
        },
        {
            "mt32.partials",
            "声音 (MT-32)：最大音部数",
            "同时播放的最大音部数, 32等同于真实MT-32硬件。\n"
            "降低此值可提高性能，代价是音符被很快切断。\n"
            "增加此值可使音符比真实硬件保持更长的时间，代价是牺牲性能。",
            {
                8,
                9,
                10,
                11,
                12,
                14,
                16,
                20,
                24,
                28,
                32,
                40,
                48,
                56,
                64,
                72,
                80,
                96,
                112,
                128,
                144,
                160,
                176,
                192,
                224,
                256,
            },
            32
        },
        {
            "mt32.dac",
            "声音 (MT-32)：DAC输入模拟模式",
            "高质量：以双倍音量产生采样，不做任何取巧，比真实设备质量更高。\n"
            "单纯: 产生和模拟LA32输出完全匹配的采样。比DAC修改（简单的削除范围内的取样）\n"
            "有更好地抗过载特性，相对其他模式更加不容易过载。音量是其他模式的一半。\n"
            "类型 1: 以早期MT-32的输出方式重新排序LA32比特输出。\n"
            "类型 2: 以后期MT-32和CM-32Ls的输出方式重新排序LA32的比特输出。",
            {
                { 0, "高质量" },
                { 1, "单纯" },
                { 2, "类型 1" },
                { 3, "类型 2" },
            },
            0
        },
        {
            "mt32.analog",
            "声音 (MT-32)：模拟输出模式",
            "数字：只模拟数字路径。输出采样只反映DAC入口的数字输出信号，这是最快的模式。\n"
            "粗略：粗略模拟LPF电路。高频被增强，采样率保持不变。声音更好一点，速度更慢一点。\n"
            "精确：更精确地模拟LPF电路。输出信号采样率提高到48kHz，\n"
            "以允许模拟可听见的16kHz以上的镜像频谱，这些信号通过LPF电路不会产生显著衰减。\n"
            "声音效果更接近于真实硬件的模拟输出，但是比“数字”和“粗略”模式更慢。\n"
            "过采样：和“精确”相同，但是输出信号被2倍过采样，即96kHz。此模式比所有其他模式都慢，\n"
            "但是在DOSBox混音器中进一步重采样时，能更好的保留高频信号。",
            {
                { 0, "数字" },
                { 1, "粗略" },
                { 2, "精确" },
                { 3, "过采样" },
            },
            2
        },
        {
            "mt32.reverb.mode",
            "声音 (MT-32)：混响模式",
            "混响模拟模式。“自动”模式自动调整混响模式以匹配加载的控制ROM版本。",
            {
                { "auto", "自动" },
                { 0, "房间" },
                { 1, "大厅" },
                { 2, "广场" },
                { 3, "节拍延迟" },
            },
            "auto"
        },
        {
            "mt32.reverb.time",
            "声音 (MT-32)：混响衰减时间",
            {
                0,
                1,
                2,
                3,
                4,
                5,
                6,
                7,
            },
            5
        },
        {
            "mt32.reverb.level",
            "声音 (MT-32)：混响级别",
            {
                0,
                1,
                2,
                3,
                4,
                5,
                6,
                7,
            },
            3
        },
        {
            "mt32.rate",
            "声音 (MT-32)：采样率",
            {
                { 8000, "8kHz" },
                { 11025, "11.025kHz" },
                { 16000, "16kHz" },
                { 22050, "22.05kHz" },
                { 32000, "32kHz" },
                { 44100, "44.1kHz" },
                { 48000, "48kHz" },
                { 49716, "49.716kHz" },
            },
            44100
        },
        {
            "mt32.src.quality",
            "声音 (MT-32)：重采样质量",
            {
                { 0, "最快" },
                { 1, "快速" },
                { 2, "好" },
                { 3, "最好" },
            },
            2
        },
        {
            "mt32.niceampramp",
            "声音 (MT-32)：高质量增益曲线",
            "增强增益曲线以保持音乐平稳。在真实硬件上，快速改变某个MIDI通道的音量或表现可能产生增益跳跃，\n"
            "启用此项可以阻止这种情况发生，禁用此项则保持模拟的精确性。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            true
        },
        {
            "pcspeaker",
            "声音：启用PC喇叭",
            "启用PC喇叭模拟（旧式主板上的小喇叭）",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            true
        },
        {
            "tandy",
            "声音：启用Tandy声音系统",
            "启用Tandy声音系统模拟。“自动”模式只有当机器类型设为Tandy时有效。",
            {
                { "auto", "自动" },
                { "on", "启用" },
                { "off", "禁用" },
            },
            "off"
        },
        {
            "disney",
            "声音：启用Disney音源",
            "启用Disney音源模拟。",
            {
                { "on", "启用" },
                { "off", "禁用" },
            },
            "off"
        },
        {
            "blocksize",
            "声音：混音器块大小（须重启）",
            "更大的块有助于改善声音顿挫，但是会增加延迟。",
            {
                256,
                512,
                1024,
                2048,
                4096,
                8192,
            },
            1024
        },
        {
            "prebuffer",
            "声音：混音器预缓存大小（须重启）",
            "在块大小顶端保留的声音数据毫秒数。",
            {
                0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 18, 20, 22, 24, 25, 26, 28,
                30, 32, 36, 40, 44, 48, 56, 64, 72, 80, 88, 96, 100
            },
            25
        },
        {
            "ipx",
            "网络: 启用IPX",
            "启用UDP上的IPX隧道。",
            {
                { true, "启用" },
                { false, "禁用" },
            },
            false
        },
        {
            "log_method",
            "日志：输出方式",
            "向哪里发送日志输出。“前端”方式发送日志到前端程序。\n"
            "“stdout/stderr”方式打印日志输出到标准输出设备或标准错误设备\n"
            "（警告和错误发送到标准错误设备，调试和信息发送到标准输出设备）",
            {
                { "frontend", "前端" },
                { "stdout/stderr", "stdout/stderr"}
            },
            "frontend",
        },
        {
            "log_level",
            "日志：详细级别",
            {
                { "debug", "调试" },
                { "info", "信息" },
                { "warnings", "警告" },
                { "errors", "错误" },
            },
            "warnings",
        },
    }
};

// clang-format on

/*

Copyright (C) 2015-2018 Andrés Suárez
Copyright (C) 2019 Nikos Chantziaras.

This file is part of DOSBox-core.

DOSBox-core is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 2 of the License, or (at your option) any later
version.

DOSBox-core is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
DOSBox-core. If not, see <https://www.gnu.org/licenses/>.

*/
