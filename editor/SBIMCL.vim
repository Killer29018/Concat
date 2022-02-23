if exists("b:current_syntax")
    finish
endif

syntax keyword SBIMCL_Keyword dup swap over rot print cr
highlight link SBIMCL_Keyword Keyword

let b:current_syntax = "SBIMCL"
