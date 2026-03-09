# https://www.debuggingbook.org/html/DeltaDebugger.html

def remove_html_markup(s):
    tag = False 
    quote = False 
    out = "" 
    for c in s: 
        if c == '<' and not quote:
            tag = True 
        elif c == '>' and not quote:
            tag = False 
        elif c =='"' or c=="'" and tag: 
            quote = not quote 
        elif not tag: 
            out = out + c
    assert out.find('<') == -1 
    return out


test_count = 0 

def test(s): 
    global test_count 
    test_count += 1
    print(f"Test #{test_count}: '{s}' (length={len(s)})")
    try: 
        result = remove_html_markup(s) 
        if '"' in s:
            assert '"' in result
        print("PASS") 
        return "PASS" 
    except AssertionError: 
        print("FAIL") 
        return "FAIL" 


def tokenize(s):
    tokens = []
    i = 0
    while i < len(s):
        if s[i] == '<':
            j = i
            while j < len(s) and s[j] != '>':
                j += 1
            if j < len(s) and s[j] == '>':
                tokens.append(s[i:j+1]) # Complete tag
                i = j + 1
            else:
                tokens.append(s[i])     # Incomplete tag
                i += 1
        else:
            tokens.append(s[i])         # Individual character
            i += 1
    return tokens


# Token-based delta debugging
def ddmin_tokens(tokens):
    def test_tokens(tokens_to_test):
        return test(''.join(tokens_to_test))
    
    assert test_tokens(tokens) == "FAIL"
    n = 2
    current_tokens = tokens
    
    while len(current_tokens) >= 2:
        subset_length = len(current_tokens) // n
        some_complement_is_failing = False
        start = 0
        
        while start < len(current_tokens):
            end = start + subset_length
            complement = current_tokens[:start] + current_tokens[end:]
            
            if test_tokens(complement) == "FAIL":
                current_tokens = complement
                n = max(n - 1, 2)
                some_complement_is_failing = True
                break
                
            start += subset_length
        
        if not some_complement_is_failing:
            if n == len(current_tokens):
                break
            n = min(n * 2, len(current_tokens))
            
    return ''.join(current_tokens)


if __name__ == "__main__":
    html_input = '"<SELECT>foo</SELECT>"' 
    #html_input = '"<SELECT><OPTION VALUE="simplify"><OPTION VALUE ="beatify")></SELECT>"'
    #html_input ='"<b>foo</b>"' 
    #html_input = '"<div><p>Hello</p><p>World</p></div>"'
    
    print("Original input:", html_input)
    tokens = tokenize(html_input)
    print("Tokens:", tokens)
    
    minimized = ddmin_tokens(tokens)
    print("\nMinimized failing input:", minimized)
    print("Total tests performed:", test_count)

# ddmin:
# time complexity : worst case O(n^2) -> n: input character count

# token based ddmin:
# time complexity : worst case O(k^2) -> k: token count (k is much less than n)
# better runtime T(n) for specific formats
