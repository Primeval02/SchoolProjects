#!/usr/bin/env ruby

##########################
# NAME: Bradley Tate
# ADGT: Activity 5
# ORGN: CSUYB - CMPS 3500
# FILE: act05.rb
# DATE: 04/02/2023
##########################

require 'csv'

RubyVM::InstructionSequence.compile_option = {
  tailcall_optimization: true,
  trace_instruction: false
}

# Function defs
def binary_search_iter(arr, el)
  max = arr.length - 1
  min = 0
  while min <= max
    mid = (min + max) / 2
    if arr[mid] == el
      return mid
    elsif arr[mid] > el
      max = mid - 1
    else
      min = mid + 1
    end
  end
  return nil
end

def binary_search_rec(arr, min, max, el)
  if max >= 1
    mid = min + (max - 1) / 2
    if arr[mid] == el
      return mid
    elsif arr[mid] > el
      return binary_search_rec(arr, min, mid - 1, el)
    else
      return binary_search_rec(arr, mid + 1, max, el)
    end
  end
end

def quicksort(array, from=0, to=nil)
  if to == nil
    to = array.count - 1
  end
  if from >= to
    return
  end
  pivot = array[from]
  min = from
  max = to
  free = min
  while min < max
    if free == min
      if array[max] <= pivot
        array[free] = array[max]
        min += 1
        free = max
      else
        max -= 1
      end
    elsif free == max
      if array[min] >= pivot
        array[free] = array[min]
        max -= 1
        free = min
      else
        min += 1
      end
    else
      raise "Inconsistent state"
    end
  end
  array[free] = pivot
  quicksort(array, from, free - 1)
  quicksort(array, free + 1, to)
end

def quicksorttail(arr)
  quicksort(arr)
end

def fibonacci_helper(a, b, count)
  if count == 0
    a
  else
    fibonacci_helper(b, a + b, count - 1)
  end
end


def fibonacci_tailrec(n)
  fibonacci_helper(0, 1, n)
end

def fibonacci_binet(n)
  sqrt5 = Math.sqrt(5);
  fibonacci_binnet = ( ((1 + sqrt5)**n) - ((1 - sqrt5)**n) ) / ((2**n) * sqrt5)
end

def binet_accuracy() 

  error = 0
  x = 0
  y = 0
  count = 1

  while error == 0
    x = fibonacci_binet(count).round()
    y = fibonacci_tailrec(count)

    error = (x - y).abs
    count += 1;
  end

  count -= 1

end

# Calling & Output
step1 = binet_accuracy()

list1 = Array.new
list1 = File.read("list1.txt").split.map(&:to_i)
list2 = Array.new
list2 = File.read("list2.txt").split.map(&:to_i)

puts "Part 1:"
puts "********\n\n"
puts "Ruby's implementation of Binet's formula is accurate until n = #{step1}\n\n" 
puts "Part 2:"
puts "********\n\n"
puts "Please input element to search: "
to_search = gets.chomp.to_i

# time for quicksort l1
starting = Process.clock_gettime(Process::CLOCK_MONOTONIC)
quicksort(list1)
ending = Process.clock_gettime(Process::CLOCK_MONOTONIC)
timeQ1 = ending - starting

# time for binary l1
starting = Process.clock_gettime(Process::CLOCK_MONOTONIC)
resbi = binary_search_iter(list1, to_search.to_i)
ending = Process.clock_gettime(Process::CLOCK_MONOTONIC)
timeB1 = ending - starting

# time for quicksort l2
starting = Process.clock_gettime(Process::CLOCK_MONOTONIC)
quicksort(list2)
ending = Process.clock_gettime(Process::CLOCK_MONOTONIC)
timeQ2 = ending - starting

# time for binary l2
starting = Process.clock_gettime(Process::CLOCK_MONOTONIC)
resbi2 = binary_search_iter(list2, to_search.to_i)
ending = Process.clock_gettime(Process::CLOCK_MONOTONIC)
timeB2 = ending - starting

list12 = Array.new
list12 = File.read("list1.txt").split.map(&:to_i)
list22 = Array.new
list22 = File.read("list2.txt").split.map(&:to_i)

# time for quicksorttail l1
starting = Process.clock_gettime(Process::CLOCK_MONOTONIC)
quicksorttail(list12)
ending = Process.clock_gettime(Process::CLOCK_MONOTONIC)
timeQT1 = ending - starting

# time for binaryrec l1
#max = list12.length - 1
#starting = Process.clock_gettime(Process::CLOCK_MONOTONIC)
#resbirec = binary_search_rec(list12, 0, max, to_search.to_i)
#ending = Process.clock_gettime(Process::CLOCK_MONOTONIC)
#timeBrec = ending - starting


puts "\nTest 1: --------------------------------------------------------------------------------"
puts "              Time to sort in seconds       Time to search an element in seconds\n"
puts "List          Recursive QuickSort           Iterative Binary Search\n"
puts "********      ************************      *************************************\n"
puts "List 1        #{timeQ1.round(8)} seconds.          #{resbi.nil? ? " Could not find #{to_search} in " : " Found #{list1[resbi]} at index #{resbi} in "}#{timeB1.round(8)} seconds."
puts "List 2        #{timeQ2.round(8)} seconds.          #{resbi2.nil? ? " Could not find #{to_search} in " : " Found #{list2[resbi2]} at index #{resbi2} in "}#{timeB2.round(8)} seconds."
puts "\nTest 2: --------------------------------------------------------------------------------"
puts "              Time to sort in seconds       Time to search an element in seconds\n"
puts "List          Tail Recursive QuickSort      Recursive Binary Search\n"
puts "********      ************************      *************************************\n"
puts "List 1        #{timeQT1.round(8)} seconds.        
     






