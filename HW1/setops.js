const fs = require('fs');

// Function that checks if a value is inside of a set
const isInside = (value, container) =>
{   // List is empty / it went through everything
    if (container.length === 0) {
        return false;
    }
    // In the list
    else if (container[0] === value) {
        return true;
    }
    // Everything else
    else {
        return isInside(value, container.slice(1));
    }
}

// Function that emulates the built in split function but can split on multiple values
const funcSplit = (inputString, splitOn) =>
{
    const splitHelper = (remainingValues, currentElement, resultList) =>
    {
        //  # If there are no more characters in the remainingValues string, add the last element to the result list
        if (remainingValues.length === 0) {
            return currentElement ? resultList.concat([currentElement]) : resultList;
        }
        // When a value to split on  is found, add the current element to the result list and recurse with the rest of the string
        else if (isInside(remainingValues[0], splitOn)) {
            return splitHelper(remainingValues.slice(1), '', resultList.concat([currentElement]));
        }
        // Append the character to the current element and recurse with the rest of the string
        else {
            return splitHelper(remainingValues.slice(1), currentElement + remainingValues[0], resultList);
        }
    }
    return splitHelper(inputString, '', []);
}

// Function that returns the legnth of the list with map and reduce
const checkLength = (input) => {
    return input.reduce((total, current) => total + 1,0);
}

// Function that removes newlines from a string
const removeNewlines = (inputItem) => {
    // # If there is a newline, it slices the string and returns the sliced string
    if (isInside('\n', inputItem)) {
        return inputItem.slice(0, -1);
    }
    // # If there isn't a newline, it just returns
    else {
        return inputItem +' ';
    }
}

//  A function that removes duplicate items in a list
const removeDuplicates = (inputList) => {
    return inputList.reduce(
        (completed, current) => {
            return isInside(current, completed) ? completed : completed.concat([current]);
        }, []);
}

//  Function that checks if the item is a character, number, or a neither
const checkType = (item) => {
    // Not number or letter
    if (!item) {
        return 'invalid';
    }
    //  Has letter in first or last index most likely a word
    else if (('a' <= item[0] && item[0] <= 'z') || ('a' <= item[item.length - 1] && item[item.length - 1] <= 'z')) {
        return 'word';
    }
    //  Has number in first or last index, most likely a number
    else if (('0' <= item[0] && item[0] <= '9') || ('0' <= item[item.length - 1] && item[item.length - 1] <= '9')) {
        return 'number';
    }
}

// Merge helper function for merge sort
const merge = (left, right) => {
    //  These check if either list is empty
    if (left.length === 0) {
        return right;
    }
    if (right.length === 0) {
        return left;
    }

    // Sets up the merge function that compares the smallest item of both lists
    const smallestLeft = left[0];
    const restLeft = left.slice(1);
    const smallestRight = right[0];
    const restRight = right.slice(1);

    // Does the merge comparison and recusively merges the remaining values
    if (smallestLeft <= smallestRight) {
        return [smallestLeft].concat(merge(restLeft, right));
    } else {
        return [smallestRight].concat(merge(left, restRight));
    }
}

// Main merge sort function that starts the sorting
const mergeSort = (inputList) => {
    const listLength = checkLength(inputList);
    if (listLength <= 1) {
        return inputList;
    }

    const middle = Math.floor(listLength / 2);
    const left = mergeSort(inputList.slice(0, middle));
    const right = mergeSort(inputList.slice(middle));

    return [left, right].reduce((leftList, rightList) => merge(leftList, rightList), []);
}

// A function that parses the sets
const setParse = (theSet) => {
    // Removes the newlines from the items in the list
    const noNewlineSet = theSet.map(removeNewlines);

    // Turns all of the characters into lowercase if it is a character
    const lowerCaseSet = noNewlineSet.map(item =>
        Array.from(item).reduce((completed, unlowered) =>
            (65 <= unlowered.charCodeAt(0) && unlowered.charCodeAt(0) <= 90) ?
            completed + String.fromCharCode(unlowered.charCodeAt(0) + 32):
            completed + unlowered[0],
            "")
    );

    // Removes the common symbols that need to be removed
    const commonSymbols = [' ', '!', '"', '#', '$', '%', '&', "'", '(', ')', '*', '+', ',', '-', '/', ':', ';', '<', '=', '>', '?', '@', '[', '\\', ']', '^', '_', '`', '{',
        '|', '}', '~', '*', '**', '/', '//', '%', '+', '-', '<', '>', '<=', '>=', '==', '!=', '=', '+=', '-=', '*=', '/=', '//=', '%='];
    const mainSplitSet = lowerCaseSet.reduce((completed, item) =>
        completed.concat(funcSplit(item, commonSymbols)),
        []);

    // Checks if the item is not empty
    const noEmptySet = mainSplitSet.filter(item => checkType(item) !== 'invalid');

    // Removes periods that are not a decimal very similar to the previous split
    const removeWordPeriodSet = noEmptySet.reduce((completed, item) =>
        (isInside('.', item) && checkType(item) === 'word') ?
        completed.concat(funcSplit(item, commonSymbols.concat(['.']))) :
        completed.concat([item]),
        []);

    return removeWordPeriodSet;
}

// Function that does intersection recursively
const intersection = (setA, setB) =>
{
    if (setA.length === 0 || setB.length === 0) {
        return [];
    }
    else if (isInside(setA[0],setB)) {
        return [setA[0]].concat(intersection(setA.slice(1), setB));
    }
    else {
        return intersection(setA.slice(1), setB);
    }
}

// Function that does difference recursively
const difference = (setA, setB) =>
{
    if (setA.length === 0) {
        return [];
    }
    else if (setB.length === 0) {
        return setA.slice();
    }
    else if (!isInside(setA[0],setB)) {
        return [setA[0]].concat(difference(setA.slice(1), setB));
    }
    else {
        return difference(setA.slice(1), setB);
    }
}

// Function that does union recursively
const union = (setA, setB) =>
{
    if (setA.length === 0) {
        return setB.slice();
    }
    else if (setB.length === 0) {
        return setA.slice();
    }
    else if (isInside(setA[0],setB)) {
        return union(setA.slice(1), setB);
    }
    else {
        return [setA[0]].concat(union(setA.slice(1), setB));
    }
}

const main = () => {
    // Grabs shell arguments and puts them into a list
    const args = process.argv.slice(2);
    if (args.length > 0) {
        shellArguments = funcSplit(args[0], [';', '=', '\r']);
    }
    else {
        console.log('Error: No system arguments were given');
        return;
    }

    // Sets up set 1, check if file exists, and checks if it is empty
    if (fs.existsSync(shellArguments[1])) {
        set1 = funcSplit(fs.readFileSync(shellArguments[1], 'utf8'),['\n','\r']);
        if (set1.length === 0) {
            console.log(`Set1Error: The file "${shellArguments[1]}" is empty`);
            return;
        }
    }
    else {
        console.log(`Set1Error: The file "${shellArguments[1]}" does not exist`);
        return;
    }

    // Sets up set 2, checks if it exists, and checks if it is empty
    if (fs.existsSync(shellArguments[3])) {
        set2 = funcSplit(fs.readFileSync(shellArguments[3], 'utf8'),['\n','\r']);
        if (set2.length === 0) {
            console.log(`Set2Error: The file "${shellArguments[3]}" is empty`);
            return;
        }
    }
    else {
        console.log(`Set2Error: The file "${shellArguments[3]}" does not exist`);
        return;
    }

    // Sets up operation type
    const operationType = shellArguments[shellArguments.length - 1];
    if (!['union', 'difference', 'intersection'].includes(operationType)) {
        console.log(`OperationError: The operation "${operationType}" is not a valid operation`);
        return;
    }

    // Parses and sorts the input sets
    const parsedSet1 = removeDuplicates(mergeSort(setParse(set1)));
    const parsedSet2 = removeDuplicates(mergeSort(setParse(set2)));

    // Does the set operation to the given sets
    let result;
    if (operationType === 'intersection') {
        result = mergeSort(intersection(parsedSet1, parsedSet2));
    } else if (operationType === 'difference') {
        result = mergeSort(difference(parsedSet1, parsedSet2));
    } else { // this is union, because we removed all of the other options
        result = mergeSort(union(parsedSet1, parsedSet2));
    }

    // Writes the output file
    fs.writeFileSync('result.txt', result.length > 0 ? result.join('\n') : 'empty set');
}

// Runs the main function
main();
