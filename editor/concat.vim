if exists("b:current_syntax")
    finish
endif

syntax keyword concat_PreProc macro endmacro
syntax match concat_PreProc "\(^macro\s\)\@<=\S\+"
highlight link concat_PreProc PreProc

syntax keyword concat_Identifier mem endmem
syntax match concat_Identifier "\(^mem\s\)\@<=\S\+"
highlight link concat_Identifier Identifier

syntax keyword concat_Keyword dup drop swap over rot mod
syntax keyword concat_Keyword print cr 
syntax keyword concat_Keyword if then elseif else endif
syntax keyword concat_Keyword while do endwhile
syntax keyword concat_Keyword and or invert land lor lnot << >>
syntax keyword concat_Keyword true false
highlight link concat_Keyword Keyword 

syntax region concat_Constants start=/^\d/ end=/\v(\ze\s|$)/
syntax region concat_Constants start=/ \d/ end=/\v(\ze\s|$)/
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
