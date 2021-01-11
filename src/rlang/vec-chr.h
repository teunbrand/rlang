#ifndef RLANG_VECTOR_CHR_H
#define RLANG_VECTOR_CHR_H

#include <string.h>


#define r_strings_na R_NaString

extern sexp* r_chrs_empty;
extern sexp* r_strings_empty;


static inline
sexp* r_chr_get(sexp* chr, r_ssize i) {
  return STRING_ELT(chr, i);
}
static inline
void r_chr_poke(sexp* chr, r_ssize i, sexp* elt) {
  SET_STRING_ELT(chr, i, elt);
}

static inline
const char* r_str_c_string(sexp* str) {
  return CHAR(str);
}

static inline
const char* r_chr_get_c_string(sexp* chr, r_ssize i) {
  return CHAR(r_chr_get(chr, i));
}

bool r_chr_has(sexp* chr, const char* c_string);
bool r_chr_has_any(sexp* chr, const char** c_strings);
r_ssize r_chr_detect_index(sexp* chr, const char* c_string);

void r_chr_fill(sexp* chr, sexp* value);

sexp* r_new_character(const char** strings);

static inline
sexp* r_string(const char* c_string) {
  return Rf_mkChar(c_string);
}

static inline
sexp* r_chr(const char* c_string) {
  return Rf_mkString(c_string);
}


sexp* r_str_unserialise_unicode(sexp* r_string);

static inline
bool r_is_string(sexp* x) {
  return
    r_typeof(x) == r_type_character &&
    r_length(x) == 1 &&
    r_chr_get(x, 0) != r_strings_na;
}

static inline
sexp* r_str_as_character(sexp* x) {
  return Rf_ScalarString(x);
}

/*
 * A symbol is always in the native encoding. This means that UTF-8
 * data frame names undergo a lossy translation when they are
 * transformed to symbols to create a data mask. To deal with this, we
 * translate all serialised unicode tags back to UTF-8. This way the
 * UTF-8 -> native -> UTF-8 translation that occurs during the
 * character -> symbol -> character conversion fundamental for data
 * masking is transparent and lossless for the end user.
 *
 * Starting from R 4.0, `installChar()` warns when translation to
 * native encoding is lossy. This warning is disruptive for us since
 * we correctly translate strings behind the scene. To work around
 * this, we call `translateChar()` which doesn't warn (at least
 * currently). If the pointers are the same, no translation is
 * needed and we can call `installChar()`, which preserves the
 * current encoding of the string. Otherwise we intern the symbol
 * with `install()` without encoding.
 */
static inline
sexp* r_str_as_symbol(sexp* str) {
  const char* str_native = Rf_translateChar(str);

  if (str_native == CHAR(str)) {
    return Rf_installChar(str);
  } else {
    return Rf_install(str_native);
  }
}

static inline
sexp* r_chr_as_symbol(sexp* str) {
  return r_sym(Rf_translateChar(r_chr_get(str, 0)));
}

static inline
bool r_str_is_name(sexp* str) {
  if (str == r_strings_na) {
    return false;
  }
  if (str == r_strings_empty) {
    return false;
  }
  return true;
}


#endif
