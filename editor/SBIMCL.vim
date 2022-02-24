if exists("b:current_syntax")
    finish
endif

syntax keyword SBIMCL_PreProc macro endmacro
syntax match SBIMCL_PreProc "\(^macro\s\)\@<=\S\+"
highlight link SBIMCL_PreProc PreProc

syntax keyword SBIMCL_Keyword dup swap over rot print cr mod if then endif
syntax keyword SBIMCL_Keyword and or invert land lor lnot
highlight link SBIMCL_Keyword Keyword 

syntax match SBIMCL_Constants "\v([0-9]+) "
highlight link SBIMCL_Constants Constant

syntax match SBIMCL_Operator "\v\*"     " *
syntax match SBIMCL_Operator "\v/"      " /
syntax match SBIMCL_Operator "\v\+"     " +
syntax match SBIMCL_Operator "\v-"      " -

syntax match SBIMCL_Operator "\v\=\="   " ==
syntax match SBIMCL_Operator "\v\!\="   " !=
syntax match SBIMCL_Operator "\v\>"     " >
syntax match SBIMCL_Operator "\v\<"     " <
syntax match SBIMCL_Operator "\v\>\="   " >=
syntax match SBIMCL_Operator "\v\<\="   " <=

syntax match SBIMCL_Operator "\v\."     " .

highlight link SBIMCL_Operator Operator

syntax match SBIMCL_Comments "\v//.*$"
syntax region SBIMCL_Comments start=/\v\/\*/ skip=/\v\\./ end=/\v\*\//
highlight link SBIMCL_Comments Comment

syntax match SBIMCL_Todo "\v(INFO|TODO|FIXME)" containedin=ALL
highlight link SBIMCL_Todo Todo

let b:current_syntax = "SBIMCL"
