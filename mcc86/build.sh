#cc -c -o ../bin/y.tab.elf ../misc/y.tab.c
#cc -c -o ../bin/lex.yy.elf ../misc/lex.yy.c

# misc/lexparsr
cc -c -o ../bin/lp.constant_token.elf ../misc/lexparsr/constant_token.c     -O2
cc -c -o ../bin/lp.directive_token.elf ../misc/lexparsr/directive_token.c   -O2
cc -c -o ../bin/lp.identifier_token.elf ../misc/lexparsr/identifier_token.c -O2
cc -c -o ../bin/lp.keyword_token.elf ../misc/lexparsr/keyword_token.c       -O2
cc -c -o ../bin/lp.punctuator_token.elf ../misc/lexparsr/punctuator_token.c -O2
cc -c -o ../bin/lp.string_token.elf ../misc/lexparsr/string_token.c         -O2
cc -c -o ../bin/lp.token.elf ../misc/lexparsr/token.c                       -O2
cc -c -o ../bin/lp.parser.elf ../misc/lexparsr/parser.c                     -O2

# local
cc -c -o ../bin/compiler.elf compiler.c                                     -O2
cc -c -o ../bin/mcc.c.elf mcc.c

cc ../bin/lp.constant_token.elf ../bin/lp.directive_token.elf ../bin/lp.identifier_token.elf \
   ../bin/lp.keyword_token.elf ../bin/lp.punctuator_token.elf ../bin/lp.string_token.elf \
   ../bin/lp.token.elf ../bin/lp.parser.elf ../bin/mcc.c.elf -o ../bin/mcc


cc -o ../bin/mcc.elf ../bin/y.tab.elf ../bin/lex.yy.elf ../bin/mcc.c.elf -ll -ly