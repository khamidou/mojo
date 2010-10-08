#!/usr/bin/env python
#
# A translator from mojo to C

import sys
import re

symtab = {}

class Symbol:
	def __init__(self, name, value):
		self.name = name
		self.value = value
class Ast:
	def __init__(self, type, value):
		self.type = type
		self.value = value
		self.method = ""

	def __repr__(self):
		return "[type: %s, value: %s, method called: %s]" % (self.type, self.value, self.method)

def read_file(file):
	return open(file).read()

def error(msg):
	print msg
	sys.exit(-1)

# The lexer class comes from http://www.gooli.org/blog/a-simple-lexer-in-python/
class Lexer(object):
    def __init__(self, definitions):
        self.definitions = definitions
        parts = []
        for name, part in definitions:
            parts.append("(?P<%s>%s)" % (name, part))
        self.regexpString = "|".join(parts)
        self.regexp = re.compile(self.regexpString, re.MULTILINE)

    def parse(self, text):
        # yield lexemes
        for match in self.regexp.finditer(text):
            found = False
            for name, rexp in self.definitions:
                m = match.group(name)
                if m is not None:
                    yield (name, m)
                    break


def parse(s):	
	defs = [("lparen", "\("),
		("rparen", "\)"),
		("assignment", ":="),
		("word", "[a-z]+"),
		("number", "[0-9]+"),]


	lex = Lexer(defs)
	input = lex.parse(s)
	prev = None
	ast = []
	
	def recparse(token):
		if token[0] == "word":
			retast = Ast("word", token[1])

			try:
				ntok = input.next()
			except StopIteration:
				return retast

			if ntok[0] == "assignment":
				retast.method = ":="
			elif ntok[0] == "word":
				retast.method = ntok[1]
			else:
				error("Invalid token %s near %s" % (ntok[1], token[1]))
			return retast

		elif token[0] == "number":
			return Ast("number", token[1])

	for token in input:
		ast.append(recparse(token))
	
	return ast

if __name__ == "__main__":
	if len(sys.argv) != 2:
		print "%s file.mj" % sys.argv[0]
		sys.exit(0)

	print parse(read_file(sys.argv[1]))


