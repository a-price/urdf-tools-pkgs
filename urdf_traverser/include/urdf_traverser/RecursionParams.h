/**
    Copyright (C) 2016 Jennifer Buehler

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software Foundation,
    Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**/
 /** 
  * Base class for recursion parameters passed during traversal of a URDF tree, and
  * a few subclasses of the base type which may be useful in various types of traversals.
  */
#ifndef URDF_TRAVERSER_RECURSIONPARAMS_H
#define URDF_TRAVERSER_RECURSIONPARAMS_H
// Copyright Jennifer Buehler

#include <baselib_binding/SharedPtr.h>
#include <urdf_traverser/Types.h>

namespace urdf_traverser
{

/**
 * \brief Base class for recursion parameters passed during traversal of a URDF tree.
 * Encapsulates data carried within a recursion. At each recursion, the
 * according fields \e link and \e level
 * are set. Any subclass can add their own recursion parameters to pass
 * through all recursions. A subclass of this type may for example 
 * be used to build the result of the traversal.
 *
 * \author Jennifer Buehler
 */
class RecursionParams
{
friend class UrdfTraverser;
public:
    typedef baselib_binding::shared_ptr<RecursionParams>::type Ptr;

    explicit RecursionParams(): level(-1) {}
    explicit RecursionParams(const RecursionParams& o):
        link(o.link),
        level(o.level){}
    virtual ~RecursionParams() {}

    RecursionParams& operator=(const RecursionParams& o)
    {
        link = o.link;
        level = o.level;
        return *this;
    }

    LinkPtr getLink() const
    {
        return link;
    }

    /**
     * Returns the current level in the tree (distance to link on which traversal was started)
     */
    unsigned int getLevel() const
    {
        return level;
    }

 protected:
    explicit RecursionParams(LinkPtr& _link, unsigned int _level):
        link(_link),
        level(_level) {}


    // Sets the current variables for the recursion state.
    void setParams(const LinkPtr& _link, int _level)
    {
        link = _link;
        level = _level;
    }

    // the link the recursion is applied on
    LinkPtr link;
    
    // level in the tree (distance to link on which traversal was started)
    unsigned int level;
};

typedef RecursionParams::Ptr RecursionParamsPtr;

/**
 * \brief Recursion parameters including link to the underlying URDF model.
 * The underlying urdf model can be used to add/remove links during traversal,
 * and/or change the root link during or after traversal.
 *
 * \author Jennifer Buehler
 */
class ModelRecursionParams: public RecursionParams
{
public:
    typedef baselib_binding::shared_ptr<ModelRecursionParams>::type Ptr;
    explicit ModelRecursionParams(): RecursionParams() {}
    explicit ModelRecursionParams(urdf::Model& _model):
        RecursionParams(),
        model(_model) {}
    ModelRecursionParams(const ModelRecursionParams& o):
        RecursionParams(o),
        model(o.model) {}
    virtual ~ModelRecursionParams() {}

    urdf::Model model;
};
typedef ModelRecursionParams::Ptr ModelRecursionParamsPtr;


/**
 * \brief Includes a factor value to be passed on in recursion.
 * \author Jennifer Buehler
 */
class FactorRecursionParams: public RecursionParams
{
public:
    typedef baselib_binding::shared_ptr<FactorRecursionParams>::type Ptr;
    explicit FactorRecursionParams(): RecursionParams(), factor(1.0) {}
    explicit FactorRecursionParams(double _factor):
        RecursionParams(),
        factor(_factor) {}
    FactorRecursionParams(const FactorRecursionParams& o):
        RecursionParams(o),
        factor(o.factor) {}
    virtual ~FactorRecursionParams() {}

    double factor;
};
typedef FactorRecursionParams::Ptr FactorRecursionParamsPtr;

/**
 * \brief Includes a flag to be passed on in recursion.
 * \author Jennifer Buehler
 */
class FlagRecursionParams: public RecursionParams
{
public:
    typedef baselib_binding::shared_ptr<FlagRecursionParams>::type Ptr;
    explicit FlagRecursionParams(): RecursionParams(), flag(false) {}
    explicit FlagRecursionParams(bool _flag):
        RecursionParams(),
        flag(_flag) {}
    FlagRecursionParams(const FlagRecursionParams& o):
        RecursionParams(o),
        flag(o.flag) {}
    virtual ~FlagRecursionParams() {}

    bool flag;
};
typedef FlagRecursionParams::Ptr FlagRecursionParamsPtr;



/**
 * \brief Collects string values into a vector
 * \author Jennifer Buehler
 */
class StringVectorRecursionParams: public RecursionParams
{
public:
    typedef baselib_binding::shared_ptr<StringVectorRecursionParams>::type Ptr;
    explicit StringVectorRecursionParams(const bool _skipFixed):
        RecursionParams(),
        skipFixed(_skipFixed) {}
    StringVectorRecursionParams(const StringVectorRecursionParams& o):
        RecursionParams(o),
        names(o.names),
        skipFixed(o.skipFixed) {}
    virtual ~StringVectorRecursionParams() {}
    // skip the fixed joints and collect only movable ones
    bool skipFixed;
    std::vector<std::string> names;
private:
    explicit StringVectorRecursionParams(){}
};
typedef StringVectorRecursionParams::Ptr StringVectorRecursionParamsPtr;


/**
 * \brief Can be used for all recursive functions which have a specific link as a result.
 * \author Jennifer Buehler
 */
class LinkRecursionParams: public RecursionParams
{
public:
    typedef baselib_binding::shared_ptr<LinkRecursionParams>::type Ptr;
    explicit LinkRecursionParams(): RecursionParams() {}
    LinkRecursionParams(const LinkRecursionParams& o):
        RecursionParams(o),
        resultLink(o.resultLink) {}
    virtual ~LinkRecursionParams() {}

    LinkPtr resultLink;
};
typedef LinkRecursionParams::Ptr LinkRecursionParamsPtr;


}  // namespace
#endif  //  URDF_TRAVERSER_RECURSIONPARAMS_H
