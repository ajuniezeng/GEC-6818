const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{ .default_target = .{
        .cpu_arch = .arm,
        .os_tag = .linux,
        .abi = .gnueabi,
    } });
    const mode = b.standardOptimizeOption(.{});

    const module = b.addModule("main", .{
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });

    module.addCSourceFiles(.{
        .files = &.{ "app/main.c", "src/lcd_control.c" },
        .flags = &.{ "-std=c11", "-Wall", "-Wextra", "-Wshadow", "-Wpedantic", "-pthread", "-Os" },
    });

    module.addIncludePath(b.path("src"));

    const artifact = b.addExecutable(.{
        .name = "GEC-6818",
        .root_module = module,
    });

    b.installArtifact(artifact);
}
