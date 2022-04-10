if exists("b:current_syntax")
    finish
endif

syntax keyword concat_Identifier mem endmem
syntax keyword concat_Identifier const
syntax keyword concat_Identifier var endvar
highlight link concat_Identifier Identifier

syntax keyword concat_Keyword dup drop swap over rot mod
syntax keyword concat_Keyword print cr 
syntax keyword concat_Keyword and or invert land lor lnot << >>
syntax keyword concat_Keyword include
syntax keyword concat_Keyword input
syntax keyword concat_Keyword cast(int) cast(bool) cast(char) cast(string) cast(memptr)
syntax match concat_Keyword "\v--\>"    " -->
highlight link concat_Keyword Keyword

syntax keyword concat_Typedef int bool char string memptr
highlight link concat_Typedef Typedef

syntax keyword concat_Function func define endfunc
highlight link concat_Function Function

syntax keyword concat_Conditional if then elseif else endif
highlight link concat_Conditional Conditional

syntax keyword concat_Repeat while do endwhile
highlight link concat_Repeat Repeat

syntax region concat_Constants start=/^\d/ end=/\v(\ze\s|$)/
syntax region concat_Constants start=/ \d/ end=/\v(\ze\s|$)/
syntax keyword concat_Constants true false
highlight link concat_Constants Constant

syntax match concat_Operator "\v\*"     " *
syntax match concat_Operator "\v/"      " /
syntax match concat_Operator "\v\+"     " +
syntax match concat_Operator "\v-"      " -

syntax match concat_Operator "\v\=\="   " ==
syntax match concat_Operator "\v\!\="   " !=
syntax match concat_Operator "\v\>"     " >
syntax match concat_Operator "\v\<"     " <
syntax match concat_Operator "\v\>\="   " >=
syntax match concat_Operator "\v\<\="   " <=

syntax match concat_Operator "\v\."     " .

highlight link concat_Operator Operator

highlight link concat_Cast Operator

syntax match concat_Comments "\v//.*$"
syntax region concat_Comments start=/\v\/\*/ skip=/\v\\./ end=/\v\*\//
highlight link concat_Comments Comment

syntax region concat_Character start=/\v\'/ end=/\v\'/
highlight link concat_Character Character

syntax region concat_String start=/\v\"/ end=/\v\"/
highlight link concat_String String

syntax match concat_Todo "\v(INFO|TODO|FIXME)" containedin=ALL
highlight link concat_Todo Todo

let b:current_syntax = "concat"
