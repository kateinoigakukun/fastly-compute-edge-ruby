#include "ruby.h"
#include <stdint.h>

typedef int32_t fastly_status_t;
typedef int32_t response_handle_t;
typedef int32_t body_handle_t;

enum body_write_end : uint32_t {
  body_write_end_back = 0,
  body_write_end_front = 1,
};

__attribute__((__import_module__("fastly_http_body"),
               __import_name__("new")))
fastly_status_t fastly_http_body_new(body_handle_t *body_handle);

__attribute__((__import_module__("fastly_http_body"),
               __import_name__("write")))
fastly_status_t fastly_http_body_write(body_handle_t body_handle, const uint8_t *buf, size_t buf_len, enum body_write_end end, size_t *nwritten_out);


__attribute__((__import_module__("fastly_http_resp"),
               __import_name__("new")))
fastly_status_t fastly_http_resp_new(response_handle_t *resp_handle);

__attribute__((__import_module__("fastly_http_resp"),
               __import_name__("send_downstream")))
fastly_status_t fastly_http_resp_send_downstream(response_handle_t resp_handle, body_handle_t body_handle, int32_t streaming);

VALUE rb_mFastly;

static VALUE _fastly_http_body_new(VALUE obj)
{
  body_handle_t body_handle;
  fastly_status_t status = fastly_http_body_new(&body_handle);
  if (status != 0) {
    rb_raise(rb_eRuntimeError, "fastly_http_body_new failed");
  }
  return INT2NUM(body_handle);
}

static VALUE _fastly_http_body_write(VALUE obj, VALUE body_handle, VALUE buf, VALUE end)
{
  body_handle_t handle = NUM2INT(body_handle);
  const uint8_t *buf_ptr = (const uint8_t *)RSTRING_PTR(buf);
  size_t buf_len = RSTRING_LEN(buf);
  enum body_write_end write_end = NUM2INT(end);
  size_t nwritten;
  fastly_status_t status = fastly_http_body_write(handle, buf_ptr, buf_len, write_end, &nwritten);
  if (status != 0) {
    rb_raise(rb_eRuntimeError, "fastly_http_body_write failed");
  }
  return INT2NUM(nwritten);
}

static VALUE _fastly_http_resp_new(VALUE obj) {
  response_handle_t resp_handle;
  fastly_status_t status = fastly_http_resp_new(&resp_handle);
  if (status != 0) {
    rb_raise(rb_eRuntimeError, "fastly_http_resp_new failed");
  }
  return INT2NUM(resp_handle);
}

static VALUE _fastly_http_resp_send_downstream(VALUE obj, VALUE resp_handle, VALUE body_handle, VALUE streaming) {
  response_handle_t handle = NUM2INT(resp_handle);
  body_handle_t body_handle_handle = NUM2INT(body_handle);
  int32_t is_streaming = NUM2INT(streaming);
  fastly_status_t status = fastly_http_resp_send_downstream(handle, body_handle_handle, is_streaming);
  if (status != 0) {
    rb_raise(rb_eRuntimeError, "fastly_http_resp_send_downstream failed");
  }
  return Qnil;
}

void
Init_fastly_compute_edge(void)
{
  printf("load Init_fastly_compute_edge\n");
  rb_mFastly = rb_define_module("Fastly");
  VALUE rb_mResponse = rb_define_module_under(rb_mFastly, "Response");
  VALUE rb_mBody = rb_define_module_under(rb_mFastly, "Body");

  rb_define_module_function(rb_mResponse, "fastly_http_resp_new", _fastly_http_resp_new, 0);
  rb_define_module_function(rb_mResponse, "fastly_http_resp_send_downstream", _fastly_http_resp_send_downstream, 3);
  rb_define_module_function(rb_mBody, "fastly_http_body_new", _fastly_http_body_new, 0);
  rb_define_module_function(rb_mBody, "fastly_http_body_write", _fastly_http_body_write, 3);
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
