#ifndef HELPER_MACRO_H
#define HELPER_MACRO_H

/*
  The kernel version I'm using for my VM does not have access to this macro yet
  (uname -r == 5.15.0-75-generic). So I decided to add it here.

  From reference:
  https://elixir.bootlin.com/linux/v5.19.17/source/tools/lib/bpf/bpf_tracing.h#L516
// */


#define ___bpf_syscall_args0()           ctx
#define ___bpf_syscall_args1(x)          ___bpf_syscall_args0(), (void *)PT_REGS_PARM1_CORE_SYSCALL(regs)
#define ___bpf_syscall_args2(x, args...) ___bpf_syscall_args1(args), (void *)PT_REGS_PARM2_CORE_SYSCALL(regs)
#define ___bpf_syscall_args3(x, args...) ___bpf_syscall_args2(args), (void *)PT_REGS_PARM3_CORE_SYSCALL(regs)
#define ___bpf_syscall_args4(x, args...) ___bpf_syscall_args3(args), (void *)PT_REGS_PARM4_CORE_SYSCALL(regs)
#define ___bpf_syscall_args5(x, args...) ___bpf_syscall_args4(args), (void *)PT_REGS_PARM5_CORE_SYSCALL(regs)
#define ___bpf_syscall_args(args...)     ___bpf_apply(___bpf_syscall_args, ___bpf_narg(args))(args)

/*
 * BPF_KPROBE_SYSCALL is a variant of BPF_KPROBE, which is intended for
 * tracing syscall functions, like __x64_sys_close. It hides the underlying
 * platform-specific low-level way of getting syscall input arguments from
 * struct pt_regs, and provides a familiar typed and named function arguments
 * syntax and semantics of accessing syscall input parameters.
 *
 * Original struct pt_regs* context is preserved as 'ctx' argument. This might
 * be necessary when using BPF helpers like bpf_perf_event_output().
 *
 * This macro relies on BPF CO-RE support.
 */
#define BPF_KPROBE_SYSCALL(name, args...)				    \
name(struct pt_regs *ctx);						    \
static __attribute__((always_inline)) typeof(name(0))			    \
____##name(struct pt_regs *ctx, ##args);				    \
typeof(name(0)) name(struct pt_regs *ctx)				    \
{									    \
	struct pt_regs *regs = PT_REGS_SYSCALL_REGS(ctx);		    \
	_Pragma("GCC diagnostic push")					    \
	_Pragma("GCC diagnostic ignored \"-Wint-conversion\"")		    \
	return ____##name(___bpf_syscall_args(args));			    \
	_Pragma("GCC diagnostic pop")					    \
}									    \
static __attribute__((always_inline)) typeof(name(0))			    \
____##name(struct pt_regs *ctx, ##args)

#endif
