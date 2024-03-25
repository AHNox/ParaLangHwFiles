const lis = ["z", "a", "t", "c", "b", "g"];
const lis2 = ["t", "x", "c", "a", "r", "l"];

const isInside = (value, container) => {
	if (container.length === 0) {
		return false;
	} else if (container[0] === value) {
		return true;
	} else {
		return isInside(value, container.slice(1));
	}
};

const setUnion = (setA, setB) => {
	if (setA.length === 0) {
		return setB.slice();
	} else if (setB.length === 0) {
		return setA.slice();
	} else if (isInside(setA[0], setB)) {
		return setUnion(setA.slice(1), setB);
	} else {
		return [setA[0]].concat(setUnion(setA.slice(1), setB));
	}
};

const setDifference = (setA, setB) => {
	if (setA.length === 0) {
		return [];
	} else if (setB.length === 0) {
		return setA.slice();
	} else if (!isInside(setA[0], setB)) {
		return [setA[0]].concat(setDifference(setA.slice(1), setB));
	} else {
		return setDifference(setA.slice(1), setB);
	}
};

const setIntersection = (setA, setB) => {
	if (setA.length === 0 || setB.length === 0) {
		return [];
	} else if (isInside(setA[0], setB)) {
		return [setA[0]].concat(setIntersection(setA.slice(1), setB));
	} else {
		return setIntersection(setA.slice(1), setB);
	}
};

console.log(process.argv[2]);
console.log(setIntersection(lis, lis2));
