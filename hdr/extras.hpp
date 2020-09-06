/*
** extras.hpp
**
** This file is part of RooleChest and Roole.
**
** Copyright (C) 2019-2020 Kyonides-Arkanthes <kyonides@gmail.com>
**
** RooleChest and Roole are free software: you can redistribute it and/or
** modify under the terms of the GNU General Public License as published by
** by the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** RooleChest and Roole are distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with RooleChest or Roole.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EXTRAS_H
#define EXTRAS_H

#include <ruby.h>

#define RMF(func) ((VALUE (*)(ANYARGS))(func))
#define RPF(func) ((VALUE (*)(VALUE))(func))
#define rstr(x) rb_str_new_cstr(x)
#define rb_ary_c_ary(a) RARRAY(a)->as.heap.ptr

#define ARRAY_TYPE_P(obj) (!RB_SPECIAL_CONST_P(obj) && \
  RB_BUILTIN_TYPE(obj) == RUBY_T_ARRAY)

static VALUE rb_sym(const char* str)
{
  return rb_id2sym(rb_intern(str));
}

static VALUE rb_sym2(VALUE str)
{
  return rb_id2sym(rb_intern_str(str));
}

void safe_mkdir(VALUE dir);

#endif
