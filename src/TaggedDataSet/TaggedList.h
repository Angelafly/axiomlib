#ifndef TAGGEDLIST_H
#define TAGGEDLIST_H

#include <algorithm>
#include <iterator>

#include <boost/shared_ptr.hpp>

namespace AxiomLib {

// ������������ ������� � ������ ������.
// �������� ����� �����, ������ �� ������� ������ ��������������
// � ������������ ����������, � ����� �����, ������ �� ������� �� ������
// �������������� � ������������ ����������
template<class Tag>
class TagSetQuery {
public:
	TagSetQuery() { init(); }

	TagSetQuery(const Tag& tag) { init(); mPositiveSet->insert(tag); }

	// �������� ����, �������� �� ������ ������������ �� ��������� ����� target
	bool match (const std::set<Tag>& target) const;

	// ����������� �������� -- ������������ ��������� �����, �������
	// ������ ��������������, � ��������� �����, ������� �� ������ ��������������
	TagSetQuery<Tag> operator +(const TagSetQuery<Tag>& other) {
		TagSetQuery<Tag> result;
		make_union(*mPositiveSet, *other.mPositiveSet, *result.mPositiveSet);
		make_union(*mNegativeSet, *other.mNegativeSet, *result.mNegativeSet);
		return result;
	}

	// ������ ������� ��������� �����, ������� ������ � �� ������ ��������������
	TagSetQuery<Tag> operator -() const {
		TagSetQuery<Tag> result(*this);
		std::swap(result.mPositiveSet, result.mNegativeSet);
		return result;
	}

private:
	typedef std::set<Tag> TagSet;

	boost::shared_ptr<TagSet> mPositiveSet;
	boost::shared_ptr<TagSet> mNegativeSet;

	void init() {
		mPositiveSet = boost::shared_ptr<TagSet>(new TagSet);
		mNegativeSet = boost::shared_ptr<TagSet>(new TagSet);
	}

	static void make_union(const std::set<Tag>& set1, const std::set<Tag>& set2,
						   std::set<Tag>& out) {
		std::set_union(set1.begin(), set1.end(),
					   set2.begin(), set2.end(),
					   std::inserter(out, out.begin()));
	}
};

template<class Value>
class QueryResult {
	friend class TaggedList;
public:
	int count() const { return (int) mValues.size(); }
	Value operator[](int elementIndex) const { return mValues[(size_t)elementIndex]; }

private:
	std::deque<Value> mValues;
};

#endif // TAGGEDLIST_H
