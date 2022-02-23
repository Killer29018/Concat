if exists("b:current_syntax")
    finish
endif

syntax keyword SBIMCL_PreProc macro end
syntax match SBIMCL_PreProc "\(^macro\s\)\@<=\S\+"
highlight link SBIMCL_PreProc PreProc

syntax keyword SBIMCL_Keyword dup swap over rot print cr mod
highlight link SBIMCL_Keyword Keyword 

syntax match SBIMCL_Operator "\v\*"
syntax match SBIMCL_Operator "\v/"
syntax match SBIMCL_Operator "\v\+"
syntax match SBIMCL_Operator "\v-"
syntax match SBIMCL_Operator "\v\."
highlight link SBIMCL_Operator Operator

syntax match SBIMCL_Comments "\v//.*$"
syntax region SBIMCL_Comments start=/\v\/\*/ skip=/\v\\./ end=/\v\*\//
highlight link SBIMCL_Comments Comment

let b:current_syntax = "SBIMCL"
