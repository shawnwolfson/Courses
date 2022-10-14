/* Copyright (C) 2014-2018 Free Software Foundation, Inc.
   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */

PHDRS
{
	text PT_LOAD FLAGS (7);
	data PT_LOAD FLAGS (7);

}

SECTIONS
{
  ENTRY(hook)
  PROVIDE (__executable_start = SEGMENT_START("text-segment", 0x400000)); . = SEGMENT_START("text-segment", 0x400000) + SIZEOF_HEADERS;

  .text : { *(.text) } :text 

  /* Adjust the address for the data segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));

  .data : { *(.data) } :data
  .bss : { *(.bss) } :data

}

