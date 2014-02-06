

def puts_pre id, *args
  print "% -7s: " % id
  puts *args
end

def print_pre id, *args 
  print "% -7s: " % id
  print *args
  STDOUT.flush
end
