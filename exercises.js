/*
 * Jakob Miner
 * Various JS Practice functions
 */

/* 
 * Basic Sum Function
 */
var sum = function (arr) {
  const len = arr.length;
  var sum = 0;
  for (var i = 0; i < len; i++) {
    if (typeof arr[i] !== 'number')
        console.log('array has invalid type');
    sum += arr[i];
  }
  return sum;
};

/* remove takes an array (`arr) and returns
 * a new array which contains the elements of `arr` with `item` removed.
 * If `arr` contains more than one copy of `item`, remove both.
 * If `arr` doesn't contain `item`, return a copy of `arr`.
 */
var remove = function (arr, item) {
  var result = [];
  var j = 0;
  for (var i = 0; i < arr.length; i++){
  	if (arr[i] !== item) result[j++] = arr[i];
  }
  return result;
};

/*
 * findDuplicates returns all values in an array which occur
 * more than once.
 */
var findDuplicates = function (arr) {
  var counted = [];
  var dups = [];
  for (var i = 0; i < arr.length; i++){
  	if (counted.includes(arr[i]) && !dups.includes(arr[i])){
    	dups.push(arr[i]);
    }
    else counted.push(arr[i]);
  }
  return dups;
};

/*
 * Map takes in two arguments - an array to perform the mapping on, and the mapping function.
 * It applies the mapping function to all elements in the array and returns an array of results.
 */
var map = function (array, mappingFunction) {
  var results = [];
  for (var i = 0; i < array.length; i++){
  	results[i] = mappingFunction(array[i]);
  }
  return results;
};

/*
 * Filter takes in two arguments - an array to filter, and a filtering function. It returns a list of all
 * elements of the array such that filterFunction(element) === true.
 */
var filter = function (array, filterFunction) {
  var result = [];
  for(var i = 0; i < array.length; i++){
  	if (filterFunction(array[i])) result.push(array[i]);
  }
  return result;
};

/*
 * Reduce takes in three arguments - an array to reduce, a reduction function, and a seed value. For every element
 * in the array, the reduction function is applied to the _current_ aggregate value and the element
 * to obtain the _new_ aggregate value. The seed value is thus the _first_ aggregate value, and the
 * result of reduce is the _final_ aggregate value obtained after processing the last element.
 */
var reduce = function (array, reductionFunction, seedValue) {
  var agg = seedValue;
  for (var i = 0; i < array.length; i++){
  	agg = reductionFunction(agg, array[i]);
  }
  return agg;
};

/* reduceRight is exactly like reduce, except that instead of processing 
 * the elements in order (left-to-right), it processes them in reverse order (right-to-left).
 */
var reduceRight = function (array, reductionFunction, seedValue) {
  var agg = seedValue;
  for (var i = array.length -1; i > -1; i--){
  	agg = reductionFunction(agg, array[i]);
  }
  return agg;
};


/*
 * For our final exercise, we're going to write a handy function that turns an input value into a
 * string. Specifically, this function will take a 'plain object' - one that has no function
 * properties - and turns it into a string like the one you'd see if you used console.log().
 *
 * There is a built-in way to do this in JavaScript; namely, JSON.stringify. However, we're going
 * to use a slightly different set of rules for our function, so you can't just call this method - sorry!
 *
 * Specifically, you should follow these rules to make a stringify your object:
 *
 * 1. NUMBERS and BOOLEANS are turned into strings directly (e.g. 2.5 -> 2.5, true -> true).
 * 2. STRINGS, as values, should have _single_ quotes around them. Don't worry about escaping
 *    characters like \n, \r, ', et cetera.
 * 3. An ARRAY should be stringified as a left square bracket ('['), followed by its stringified values separated
 *    by commas, followed by a right square bracket (']'). There should be a space following every comma, and there
 *    should be no comma following the final value.
 * 4. An OBJECT should be stringified as a left curly brace ('{'), followed by each of its key value pairs, followed
 *    by a right curly brace ('}'). A key-value pair should be stringified as the key (with no surrounding quotes),
 *    then a colon and a space, and then the stringified value. As with arrays, the key-value pairs should be
 *    separated by commas; there should be a space following every comma, and there
 *    should be no comma and space following the final value.
 * 5. NULL should be stringified as null, and UNDEFINED should be stringified as undefined (no quotes around either).
 * 6. If you ever encounter a function (as the original input value, as an array element, or as an object property),
 *    then you should throw a new Error('Illegal argument').
 *
 * HINT 1: Your function should work for ANY plain JavaScript object you pass it, including base types like
 * numbers/strings. This points towards a pretty slick recursive implementation...
 * HINT 2: Use Object.keys to get the keys of an object to iterate through.
 * HINT 3: You can use `typeof` to get the type of an object (as a string). Read the docs online
 * at https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Operators/typeof.
 * HINT 4: typeof doesn't work for Arrays (it just gives you 'object'). Use Array.isArray to differentiate them.
 *
 * ==========================================================================
 */

var stringify = function (object) {
  var result = '';
  var type = typeof object;
  
  if (object === null) return 'null';
  
  else if (Array.isArray(object)){
  	result = result + '[';
    for (var i = 0; i < object.length; i++){
    	result = result + stringify(object[i]);
      if (i < object.length - 1) result = result + ', ';
    }
    result = result + ']';
  }
  
  else if (type === 'object'){
  	result = result + '{';
    for (var i = 0; i < Object.keys(object).length; i++){
    	result = result + Object.keys(object)[i];
    	result = result + ': ';
    	result = result + stringify(object[Object.keys(object)[i]]);
      if (i < Object.keys(object).length - 1) result = result + ', ';
    }   
  	result = result + '}';
  }
  
  else if (type === 'string'){
  	result = result + "'" + object + "'";
  }
  
  else if (type === 'function'){
  	throw ('Illegal argument');
  }
  else result = result + object;
  return result;
};

/*
 * NOTE: don't change this code.
 *
 * Export solution as a module.
 */
module.exports = {
  arrayMethods: {
    sum: sum,
    remove: remove,
    findDuplicates: findDuplicates
  },
  highorderMethods: {
    map: map,
    filter: filter,
    reduce: reduce,
    reduceRight: reduceRight
  },
  stringifyMethods: {
    mystringify:stringify
  }
};

