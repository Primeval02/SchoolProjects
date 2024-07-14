#!/usr/bin/env ruby
######################################
#Fibonacci Sequence
#Tail-Call Recursive Implementation
#CS 3500
######################################

# Enable tail-call optimization here
RubyVM::InstructionSequence.compile_option = {
  tailcall_optimization: true,
  trace_instruction: false
}

#Defining helper to implement a Tail-Call Recursive Implementation
	def fibonacci_helper( a, b, count )
	  if count == 0
		a
	  else
		fibonacci_helper( b, a + b , count - 1 )
	  end
	end
#
	def fibonacci_tailrec( n )
	  fibonacci_helper( 0, 1, n )
	end
#Getting input number from user
print "Input the n-th Fibonacci Number you would like to calculate: "

#reading input as a number
n = gets.chomp.to_i

#start time to calculate elpased time
starting = Process.clock_gettime(Process::CLOCK_MONOTONIC)

#No rounding necessary
puts "The #{n}-th fiboancci number is: #{fibonacci_tailrec(n)}"

#end time to calculate elpased time
ending = Process.clock_gettime(Process::CLOCK_MONOTONIC)

#calculating elpased time
time = ending - starting
puts "Time elapsed #{time} seconds"
