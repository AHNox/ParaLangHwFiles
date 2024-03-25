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
    return reduce(lambda total, current: total + 1,input,0)

# Function that removes newlines from a string
def removeNewlines(inputItem):
    if isInside('\n',inputItem):
        # If there is a newline, it slices the string and returns the sliced string
        return inputItem[0:-1]
    else:
        # If there isn't a newline, it just returns
        return inputItem

# Function that checks if the item is a character, number, or a period
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
    left, right = map(mergeSort, [inputList[:middle], inputList[middle:]])

    return reduce(lambda acc, elem: merge(acc, elem), [left, right])

def main():
    textInput = 'aX.txt'
    with open(textInput,'r') as file: set1 = file.readlines()
    print(set1)

    # Removes the newlines from the items in the list
    newSet1 = list(map(removeNewlines,set1))

    # Turns all of the characters into lowercase if it is a character
    loweredSet = lowercase_set = list(map(lambda item: reduce(lambda acc, char: acc + chr(ord(char) + 32) if 65 <= ord(char) <= 90 else acc + char, item, ""), newSet1))

    # Removes the common symbols that need to be removed
    commonSymbols = [' ','!', '"', '#', '$', '%', '&', "'", '(', ')', '*', '+', ',', '-', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~', '*', '**', '/', '//', '%', '+', '-', '<', '>', '<=', '>=', '==', '!=', '=', '+=', '-=', '*=', '/=', '//=', '%=']
    newerSet1 = reduce(lambda completed, item:completed + funcSplit(item, commonSymbols), loweredSet, [])

    # Checks if the item is not empty
    notEmptySet1 = list(filter(lambda item: checkType(item) != 'invalid', newerSet1))

    # Removes periods that are not a decimal very similar to the previous split
    removeWordPeriodSet1 = reduce(lambda completed, item: list(completed) + funcSplit(item, commonSymbols+['.']) if isInside('.', item) and checkType(item) == 'word' else completed + [item], notEmptySet1, [])

    finalSorted = mergeSort(removeWordPeriodSet1)
    print(finalSorted)


# Runs the main function
main()