#pragma once

#include <opencv/cv.h>
#include <math.h>
#include <vector>
#include <iostream>
//#include <vnl/vnl_complex_traits.h>
//#include <vnl/algo/vnl_svd.h>
#include <gdkmm.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>


#include <string>
#include <vector>
#include <set>
#include <sstream>
#include <stdexcept>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


//#define DEBUG

using namespace std;
using namespace boost;

#define xforeach(iter,container) \
    for(decltype(container.begin()) iter = container.begin();	\
	iter != container.end(); iter++)

#define xforeachback(iter,container) \
    for(decltype(container.rbegin()) iter = container.rbegin();	\
	iter != container.rend(); iter++)

//typedef vnl_vector<double> Vector;
//typedef vnl_matrix<double> Matrix;



template <class T>
inline T square(T a) {
    return a * a;
}

template <class T>
ostream& operator<< (ostream& out, vector<T> const& vec) {
    out << vec.size() << endl;
    xforeach(iter, vec)
	out << *iter << endl;
    return out;
}

template <class T>
istream& operator>> (istream& in, vector<T> &vec) {
    int size;
    T element;

    vec.clear();
    in >> size;
    for(int i=0; i<size; i++) {
	in >> element;
	vec.push_back(element);
    }
    return in;
}


template <class T>
T teefunction(T source, char* prefix, char *postfix="\n") {
    cout << prefix << source << postfix;
    return source;
}

#define debugtee(x) teefunction(x, #x ": ")

template <class T>
void savevector(CvFileStorage *out, const char* name, vector<T>& vec) {
    cvStartWriteStruct(out, name, CV_NODE_SEQ);
    for(int i=0; i<vec.size(); i++)
	vec[i].save(out);
    cvEndWriteStruct(out);
}

template <class T>
vector<T> loadvector(CvFileStorage *in, CvFileNode *node) {
    CvSeq *seq = node->data.seq;
    CvSeqReader reader;
    cvStartReadSeq(seq, &reader, 0);
    vector<T> result(seq->total);
    for(int i=0; i<seq->total; i++) {
	CvFileNode *item = (CvFileNode*) reader.ptr;
	result[i].load(in, item);
	CV_NEXT_SEQ_ELEM(seq->elem_size, reader);
    }
    return result;
}

#include <gtkmm.h>

#include "Point.h"

template <class From, class To>
void convert(const From& from, To& to) {
    to = from;
}

template <class From, class To>
void convert(const vector<From> &from, vector<To> &to) {
    to.resize(from.size());
    for(int i=0; i< (int) from.size(); i++)
	convert(from[i], to[i]);
}

class ConstancyDetector {
    int value;
    int counter;
    int maxcounter;
public:
    ConstancyDetector(int maxcounter) :
	value(-1), counter(0), maxcounter(maxcounter) {}

    bool isStable(void) {
	return counter >= maxcounter;
    }

    bool isStableExactly(void) {
	return counter == maxcounter;
    }

    bool observe(int newvalue) {
	if (newvalue != value || newvalue < 0)
	    counter = 0;
	else
	    counter++;
	value = newvalue;
	return isStable();
    }

    void reset(void) {
	counter = 0;
	value = -1;
    }
};

// #define output(X) { cout << #X " = " << X << endl; }


template <class T>
int maxabsindex(T const& vec, int size) {
    int maxindex = 0;
    for(int i=0; i<size; i++)
	if (fabs(vec[i]) > fabs(vec[maxindex]))
	    maxindex = i;
    return maxindex;
}

namespace boost {
    template <>
	void checked_delete(IplImage *image);
}

void releaseImage(IplImage *image);
boost::shared_ptr<IplImage> createImage(const CvSize &size, int depth, int channels);

typedef boost::shared_ptr<const IplImage> SharedImage;


//----------------------------------------------------------------------------------------

#ifndef __PUPIL_TRACKER_UTILS_H__
#define __PUPIL_TRACKER_UTILS_H__


namespace pupiltracker {

class MakeString
{
public:
    std::stringstream stream;
    operator std::string() const { return stream.str(); }

    template<class T>
    MakeString& operator<<(T const& VAR) { stream << VAR; return *this; }
};

inline int pow2(int n)
{
    return 1 << n;
}

template<typename T>
inline T sq(T n)
{
    return n * n;
}

template<typename T>
inline T lerp(const T& val1, const T& val2, double alpha)
{
    return val1*(1-alpha) + val2*alpha;
}

template<typename T>
std::vector<T> randomSubset(const std::vector<T>& src, typename std::vector<T>::size_type size)
{
    if (size > src.size())
    {
        throw std::range_error("Subset size out of range");
        //return NULL;
    }

    std::vector<T> ret;
    std::set<size_t> vals;

    for (size_t j = src.size() - size; j < src.size(); ++j)
    {
        size_t idx = rand() % j;

        if (vals.find(idx) != vals.end())
            idx = j;

        ret.push_back(src[idx]);
        vals.insert(idx);
    }

    return ret;
}

template<typename T>
std::vector<T> randomSubset(const std::vector<T>& src, typename std::vector<T>::size_type size, unsigned int seed)
{
    if (size > src.size())
    {
        throw std::range_error("Subset size out of range");
        //return NULL;
    }

    std::vector<T> ret;
    std::set<size_t> vals;

    for (size_t j = src.size() - size; j < src.size(); ++j)
    {
        srand(seed+j);
        size_t idx = rand() % j;

        if (vals.find(idx) != vals.end())
            idx = j;

        ret.push_back(src[idx]);
        vals.insert(idx);
    }

    return ret;
}

} //namespace pupiltracker

#endif // __PUPIL_TRACKER_UTILS_H__

//-----------------------------------------------------------------------------------------
