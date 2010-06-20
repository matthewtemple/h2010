#ifndef h_core_content_h
#define h_core_content_h

#define H_CORE_CONTENT_TYPE_COUNT 5
enum h_core_content_t {
  H_CORE_CONTENT_UNKNOWN = 0,
  H_CORE_CONTENT_MIME_IMAGE_GIF = 1,
  H_CORE_CONTENT_MIME_TEXT_CSS = 2,
  H_CORE_CONTENT_MIME_TEXT_HTML = 3,
  H_CORE_CONTENT_MIME_TEXT_JAVASCRIPT = 4,
};
typedef enum h_core_content_t h_core_content_t;

char *h_core_content_get_name(h_core_content_t content_type);

#endif
