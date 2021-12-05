#include "ruby.h"

VALUE rb_mFastly;

static VALUE example_hello(VALUE obj)
{
  printf("hello\n");
  return Qnil;
}

void
Init_fastly_compute_edge(void)
{
  printf("load Init_fastly_compute_edge\n");
  rb_mFastly = rb_define_module("Fastly");

  rb_define_module_function(rb_mFastly, "hello", example_hello, 0);
}

#define init(func, name) {	\
    extern void func(void);	\
    ruby_init_ext(name".so", func); \
}

void ruby_init_ext(const char *name, void (*init)(void));

void Init_gemext(void) {
  printf("load Init_gemext\n");
  init(Init_fastly_compute_edge, "fastly/compute_edge");
}
