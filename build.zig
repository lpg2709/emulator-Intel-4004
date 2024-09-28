const std = @import("std");
const buildin = @import("builtin");

pub const c_source_files = &[_][]const u8{
    "src/main.c",
    "src/4004_chip.c",
    "src/a_scanner.c",
    "src/a_token.c",
    "src/application.c",
    "src/assembler.c",
    "src/config.c",
    "src/files.c",
    "src/gui_cli.c",
    "src/options_parser.c",
    "src/sleep.c",
};

pub const c_flags_debug = &[_][]const u8{ "-std=c99", "-pedantic", "-Wall", "-Wextra", "-DDEBUG=1" };

pub const c_flags_release = &[_][]const u8{"-O3"};

pub fn build(b: *std.Build) void {
    comptime {
        const zig_version = "0.13.0";
        const supported_zig = std.SemanticVersion.parse(zig_version) catch unreachable;
        if (buildin.zig_version.order(supported_zig) != .eq) {
            @compileError(std.fmt.comptimePrint("Unsupported zig version ({}). Required Zig version '{s}'!", .{ buildin.zig_version, zig_version }));
        }
    }

    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "4004-emulator",
        .target = target,
        .optimize = optimize,
        .link_libc = true,
    });

    exe.addIncludePath(b.path("src"));

    exe.addCSourceFiles(.{
        .files = c_source_files,
        .flags = switch (optimize) {
            .Debug => c_flags_debug,
            else => c_flags_release,
        },
    });

    b.installArtifact(exe);

    const run_cmd = b.addRunArtifact(exe);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run emulator after build.");
    run_step.dependOn(&run_cmd.step);
}
