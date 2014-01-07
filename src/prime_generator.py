#!/usr/bin/env python

import sys

class PrimeList(object):
    def _generate_primes(self,max_prime):
        """
        http://code.activestate.com/recipes/366178-a-fast-prime-number-list-generator/
        """
        if max_prime==2: return [2]
        elif max_prime<2: return []
        s=range(3,max_prime+1,2)
        mroot = max_prime ** 0.5
        half=(max_prime+1)/2-1
        i=0
        m=3
        while m <= mroot:
            if s[i]:
                j=(m*m-3)/2
                s[j]=0
                while j<half:
                    s[j]=0
                    j+=m
            i=i+1
            m=2*i+3
        return [2]+[x for x in s if x]
            
    def __init__(self,max_prime):
        self.primes=self._generate_primes(max_prime)

class HtablePrimesHeader(object):
    
    def __init__(self,max_prime,header_file_name):
        header_guard="_"+header_file_name.replace(".","_").upper()
        header_start="".join(
            [
                "#ifndef %s\n" % header_guard,
                "#define %s\n\n" % header_guard,
                "size_t HTABLE_PRIMES[] = \n",
                "{\n"
            ]
        )
        
        header_end="".join(
            [
                "};\n",
                "\n",
                "#endif /* %s */\n\n" % header_guard
            ]
        )
        
        prime_list_strings=[]
        prime_string="    "
        primes=PrimeList(max_prime).primes
        for prime in primes:
            
            next_part="%d" % prime
            if len(prime_string) + len(next_part) + len(",") > 75:
                prime_string += "\n"
                prime_list_strings.append(prime_string)
                prime_string="    "
            if prime != primes[-1]:
                prime_string+=next_part+","
            else:
                print "last one."
                prime_string+=next_part+"\n"
                prime_list_strings.append(prime_string)
                prime_string=None
                
        prime_list_string="".join(prime_list_strings)
        
        self.header_string=header_start + prime_list_string + header_end
    def __str__(self):
        return self.header_string
        

def main(max_prime,header_file_name):
    header=HtablePrimesHeader(max_prime,header_file_name)
    open(header_file_name,"wb").write(str(header))

if __name__ == "__main__":
    main(int(sys.argv[1]),sys.argv[2])
