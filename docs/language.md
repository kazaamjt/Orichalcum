# Language constructs

This document is being constructed as I go.  
I will write topics ordered by when I tackle them.  
I'll reorder things in the future. Maybe.  

## The lexer

The lexer is handwritten.  
I opted going for a handwritten lexer because it really isn't that much work and allows for much more control.  
It's the smallest part of the compiler by far.  

### Indentation

The language currently uses indentation to detect blocks of code.  
The lexer detects indentation.  
When a newline starts, it checks to see if the first character on said line is either `space` or `\t`.  
The lexer does not care wether the indentation is needed or mixed. (Mixed indentation is not allowed!)

## keywords

```txt
and        as        assert      async
await      break     continue    class
def        del       elif        else
except     False     finally     for
from       global    if          import
in         is        lambda      None
nonlocal   not       or          pass
raise      return    struct      switch
True       try       while       with
yield
```
