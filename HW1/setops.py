import sys, os
from functools import reduce

# Function that checks if a value is inside of a set
def isInside(value, container):
    if not container:
        # List is empty / it went through everything
        return False
    elif container[0] == value:
        # In the list
        return True
    else:
        # Everything else
        return isInside(value,container[1:])

# Function that emulates the built in split function but can split on multiple values
def funcSplit(inputString, splitOn):
    def splitHelper(remainingValues, currentElement, resultList):
        if not remainingValues:
            # If there are no more characters in the remainingValues string, add the last element to the result list
            return resultList + [currentElement] if currentElement else resultList
        elif isInside(remainingValues[0], splitOn):
            # When a value to split on  is found, add the current element to the result list and recurse with the rest of the string
            return splitHelper(remainingValues[1:], '', resultList + [currentElement])
        else:
            # Append the character to the current element and recurse with the rest of the string
            return splitHelper(remainingValues[1:], currentElement + remainingValues[0], resultList)

    return splitHelper(inputString, '', [])

# Function that returns the legnth of the list with map and reduce
def checkLength(input):
    return reduce(
                # Function: Counts number of items
                lambda total, current: total + 1,
                # List that the lambda function goes over
                input,
                # Initial value
                0)

# Function that removes newlines from a string
def removeNewlines(inputItem):
    if isInside('\n',inputItem):
        # If there is a newline, it slices the string and returns the sliced string
        return inputItem[0:-1]
    else:
        # If there isn't a newline, it just returns
        return inputItem

# A function that removes duplicate items in a list
def removeDuplictes(inputList):
    return reduce(
                # Reduce function that combines the values of the lambda function
                # Lambda function that checks if the current value is already inside of the completedlist
                lambda completed, current: completed + [current] if not isInside(current,completed) else completed,
                # The list that the reduce funciton is run on
                inputList,
                # The inital value of the reduce function
                [])

# Function that checks if the item is a character, number, or neither
def checkType(item):
    # Not number or letter
    if not item:
        return 'invalid'
    # Has letter in first or last index most likely a word
    elif 'a' <= item[0] <= 'z' or 'a' <= item[-1] <= 'z':
        return 'word'
    # Has number in first or last index, most likely a number
    elif '0' <= item[0] <= '9' or '0' <= item[-1] <= '9':
        return 'number'

# Merge helper function for merge sort
def merge(left, right):
    # These check if either list is empty
    if not left:
        return right
    if not right:
        return left

    # Sets up the merge function that compares the smallest item of both lists
    smallestLeft = left[0]
    restLeft = left[1:]
    smallestRight = right[0]
    restRight = right[1:]

    # Does the merge comparison and recusively merges the remaining values
    if smallestLeft <= smallestRight:
        return [smallestLeft] + merge(restLeft, right)
    else:
        return [smallestRight] + merge(left, restRight)

# Main merge sort function that starts the sorting
def mergeSort(inputList):
    listLength = checkLength(inputList)
    if listLength <= 1:
        return inputList

    middle = listLength // 2
    left = mergeSort(inputList[:middle])
    right = mergeSort(inputList[middle:])

    return reduce(
                # Reduce is used to combine all of the results of the merge into one list
                # Functino that runs the merge on both the right and left
                lambda leftList, rightList: merge(leftList, rightList),
                #  Left and right lists that the merge function is run through
                [left, right])

# A function that parses the sets
def setParse(theSet):
    # Removes the newlines from the items in the list
    noNewlineSet = list(map(
                            # Funcion that removes newlines
                            removeNewlines,
                            # The list that removeNewlines function
                            theSet))

    # Turns all of the characters into lowercase if it is a character
    lowerCaseSet = list(map(
                            # Function that map is running through
                            # Lambda function that combines the lowercase of all the letters
                            lambda item:
                                # Reduce combines together the characters together
                                reduce(
                                    # Lambda function that turns all of the uppercase letters to lowercase letters
                                    lambda completed, unlowered: completed + chr(ord(unlowered) + 32)
                                        # If its lower case do the above
                                        if 65 <= ord(unlowered) <= 90
                                        # If its upper case do below
                                        else completed + unlowered,
                                    # List that the reduce goes through
                                    item,
                                    # Inital item that goes into the reduce
                                    ""
                                    ),
                            # List that the lambda function goes through
                            noNewlineSet))

    # Removes the common symbols that need to be removed
    commonSymbols = [' ','!', '"', '#', '$', '%', '&', "'", '(', ')', '*', '+', ',', '-', '/',':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{',
                     '|', '}', '~', '*', '**', '/', '//', '%', '+', '-', '<', '>', '<=', '>=','==', '!=', '=', '+=', '-=', '*=', '/=', '//=', '%=']
    mainSplitSet = reduce(
                        # Reduce combines all of the split values and places them into a list
                        # lambda funciton that splits
                        lambda completed, item:completed + funcSplit(item, commonSymbols),
                        # The set that the lambda function is going to go over
                        lowerCaseSet,
                        # The initial value
                        [])

    splitWordNumberSet = reduce(
                        # Reduce that combines all of the items in the list
                        lambda completed, item: completed + (reduce(
                                    # Reduce that combines the number and word into a list
                                    lambda completed, currItem: [completed[0] + currItem, completed[1]]
                                        if 'a' <= currItem[0] <= 'z'
                                        else [completed[0], completed[1] + currItem],
                                    item,
                                ['', ''])
                            if item and (('a' <= item[0] <= 'z' and '0' <= item[-1] <= '9') or ('0' <= item[0] <= '9' and 'a' <= item[-1] <= 'z'))
                            else [item]),
                        # List that the lambda funciton goes through
                        mainSplitSet,
                        # Initial value
                        [])

    # Checks if the item is not empty
    noEmptySet = list(filter(
                        # Function that the filter function is looking for to be true
                        lambda item: checkType(item) != 'invalid',
                        # List that the filter functionis going to go over
                        splitWordNumberSet))

    # Removes periods that are not a decimal very similar to the previous split
    removeWordPeriodSet = reduce(
                                # Funciton that splits all periods in words
                                lambda completed, item:
                                    # Lambda expression that combines the current value with the split version of the word
                                    list(completed) + funcSplit(item, commonSymbols+['.'])
                                        # Conditional that checks if it has a period and is a word
                                        if isInside('.', item) and checkType(item) == 'word'
                                        # Else condition when it is not a word and doesnt have a period
                                        else completed + [item],
                                # The list that the reduce function goes through
                                noEmptySet,
                                # Inital value that is passed into the reduce functino
                                [])

    return removeWordPeriodSet

# Function that does intersection recursively
def intersect(firstSet, secondSet):
    if not firstSet or not secondSet: # Returns an empty list if either of the lists is empty
        return []
    elif isInside(firstSet[0], secondSet):
        return [firstSet[0]] + intersect(firstSet[1:], secondSet)
    else:
        return intersect(firstSet[1:], secondSet)

# Function that does difference recursively
def difference(firstSet, secondSet):
    if not firstSet:
        return []
    elif not secondSet:
        return firstSet[:]
    elif not isInside(firstSet[0], secondSet):
        return [firstSet[0]] + difference(firstSet[1:], secondSet)
    else:
        return difference(firstSet[1:], secondSet)

# Function that does union recursively
def union(firstSet, secondSet):
    # Checks if the main set is is empty then returns the other set
    if not firstSet:
        return secondSet
    elif not secondSet:
        return firstSet
    # Checks if
    elif isInside(firstSet[0], secondSet):
        return union(firstSet[1:], secondSet)
    # If the first element of set A is not in set B, include it in the result and recursively call setUnion without the first element of set A
    else:
        return [firstSet[0]] + union(firstSet[1:], secondSet)

def main():
    # Grabs shell arguments and puts them into a list
    if checkLength(sys.argv) > 1:
        shellArguments = funcSplit(sys.argv[1], [';','=','\r'])
    else:
        print('Error: No system arguments were given')
        return


    # Sets up set 1, check if file exists, and checks if it is empty
    if os.path.exists(shellArguments[1]):
        with open(shellArguments[1],'r') as file: set1 = file.readlines()
        if checkLength(set1) == 0:
            print('Set1Error: The file "' + shellArguments[1] + '" is empty')
            return
    else:
        print('Set1Error: The file "' + shellArguments[1] + '" does not exist')
        return

    # Sets up set 2, checks if it existsk, and checks if it is empty
    if os.path.exists(shellArguments[3]):
        with open(shellArguments[3],'r') as file: set2 = file.readlines()
        if checkLength(set2) == 0:
            print('Set2Error: The file "' + shellArguments[3] + '" is empty')
            return
    else:
        print('Set2Error: The file "' + shellArguments[3] + '" does not exist')
        return

    # Sets up operation type
    if isInside(shellArguments[-1], ['union', 'difference', 'intersection']):
        operationType = shellArguments[-1]
    else:
        print('OperationError: The operation "' + shellArguments[-1] + '" is not a valid operation')
        return

    # Parses and sorts the input sets
    parsedSet1 = removeDuplictes(mergeSort(setParse(set1)))
    parsedSet2 = removeDuplictes(mergeSort(setParse(set2)))

    # Does the set operations to the given sets
    if operationType == 'intersection':
        result = mergeSort(intersect(parsedSet1, parsedSet2))
    elif operationType == 'difference':
        result = mergeSort(difference(parsedSet1, parsedSet2))
    else: # this is union, becuase we removed all of the other options
        result = mergeSort(union(parsedSet1, parsedSet2))

    # Writes the output file
    with open('result.txt', 'w') as file:
        if result:
            list(map(
                    # The function that write to the output file
                    lambda currValue: file.write(str(currValue) + '\n') if currValue != result[-1] else file.write(str(currValue)),
                    # The list that the lambda function goes through
                    result))
        else:
            file.write("empty set")

# Runs the main function
main()