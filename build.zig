const std = @import("std");
const buildin = @import("builtin");

const Test = struct {
    name: []const u8,
    files: []const []const u8,
};

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

    const test_step = b.step("test", "Run unit tests");
    for (tests) |t| {
        const t_exe = b.addExecutable(.{
            .name = t.name,
            .target = target,
            .optimize = optimize,
            .link_libc = true,
        });
        t_exe.addIncludePath(b.path("src"));
        t_exe.addIncludePath(b.path("test/vendor/unity"));
        t_exe.addCSourceFile(.{ .file = b.path("test/vendor/unity/unity.c"), .flags = &[_][]const u8{} });
        t_exe.addCSourceFiles(.{
            .files = t.files,
            .flags = &[_][]const u8{},
        });
        const run_unit_tests = b.addRunArtifact(t_exe);
        test_step.dependOn(&run_unit_tests.step);
    }
}

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

pub const tests = [_]Test{
    Test{
        .name = "a_scanner",
        .files = &[_][]const u8{
            "src/a_scanner.c",
            "src/a_token.c",
            "test/a_scanner/scan_tokens.c",
            "test/a_scanner/test_runner.c",
        },
    },
    Test{
        .name = "options_parser",
        .files = &[_][]const u8{
            "src/options_parser.c",
            "test/options_parser/options_parser.c",
            "test/options_parser/test_runner.c",
        },
    },
};

pub const c_flags_debug = &[_][]const u8{ "-std=c99", "-pedantic", "-Wall", "-Wextra", "-DDEBUG=1" };

pub const c_flags_release = &[_][]const u8{"-O3"};
