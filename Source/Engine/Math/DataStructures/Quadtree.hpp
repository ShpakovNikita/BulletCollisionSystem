#pragma once
#include <vector>
#include <array>
#include "Math/AABBox2.hpp"
#include "Core/Renderer.hpp"

template<typename T>
class Quadtree
{
public:
    Quadtree(const AABBox2& bounds, const std::function<AABBox2(const T&)>& bboxCreationFunc);
    ~Quadtree();

    void Clear();
    void Insert(const T& object);
    void Remove(const T& object);
    std::vector<T> GetCollidedObjects(const T& objectToCollide) const;

    void DebugDraw(const Renderer* renderer);

private:
    struct ObjectBBoxCollision
    {
        T object;
        AABBox2 bbox;
    };

    struct Node
    {
        Node(Node* aParent, const AABBox2& aBounds)
            : parent(aParent)
            , bounds(aBounds)
        {
            childs.fill(nullptr);
        }

        void Clear();
        void Split();
        int32_t GetIndex(const AABBox2& objectBBox) const;
        void Insert(ObjectBBoxCollision&& collisionObject);
        void Remove(ObjectBBoxCollision&& collisionObject);
        void DiscardEmptyNodes();
        void GetCollidedObjects(std::vector<T> &collidedObjects, const AABBox2& collisionBBox) const;

        void DebugDraw(const Renderer* renderer);

        Node* parent = nullptr;
        AABBox2 bounds;
        std::vector<ObjectBBoxCollision> objects;
        std::array<Node*, 4> childs;
    };

    static const uint32_t kMaxObjectsInNode = 2;

    Node* rootNode = nullptr;
    std::function<AABBox2(const T&)> bboxCreationFunc;
};

template<typename T>
void Quadtree<T>::Node::GetCollidedObjects(std::vector<T>& collidedObjects, const AABBox2& collisionBBox) const
{
    if (childs[0] != nullptr)
    {
        int32_t childIndex = GetIndex(collisionBBox);
        if (childIndex != -1)
        {
            childs[childIndex]->GetCollidedObjects(collidedObjects, collisionBBox);
        }
    }

    for (const ObjectBBoxCollision& collisionObject : objects)
    {
        collidedObjects.push_back(collisionObject.object);
    }
}

template<typename T>
std::vector<T> Quadtree<T>::GetCollidedObjects(const T& objectToCollide) const
{
    std::vector<T> collidedObjects;
    rootNode->GetCollidedObjects(collidedObjects, bboxCreationFunc(objectToCollide));

    return collidedObjects;
}

template<typename T>
void Quadtree<T>::Insert(const T& object)
{
    ObjectBBoxCollision collisionObject = { object, bboxCreationFunc(object) };
    rootNode->Insert(std::move(collisionObject));
}

template<typename T>
void Quadtree<T>::Remove(const T& object)
{
    ObjectBBoxCollision collisionObject = { object, bboxCreationFunc(object) };
    rootNode->Remove(std::move(collisionObject));
}

template<typename T>
Quadtree<T>::~Quadtree()
{
    rootNode->Clear();
    delete rootNode;
    rootNode = nullptr;
}

template<typename T>
int32_t Quadtree<T>::Node::GetIndex(const AABBox2& objectBBox) const
{
    for (size_t i = 0; i < childs.size(); ++i) {
        if (objectBBox.min >= childs[i]->bounds.min &&
            objectBBox.max <= childs[i]->bounds.max)
        {
            return static_cast<int32_t>(i);
        }
    }

    return -1;
}

template<typename T>
void Quadtree<T>::Node::Remove(ObjectBBoxCollision&& collisionObject)
{
    if (childs[0] != nullptr)
    {
        int32_t childIndex = GetIndex(collisionObject.bbox);

        if (childIndex != -1)
        {
            childs[childIndex]->Remove(std::move(collisionObject));

            return;
        }
    }

    static const auto collisionObjectsComparator = [&collisionObject](const Quadtree<T>::ObjectBBoxCollision& obj)
    {
        return collisionObject.object == obj.object;
    };

    objects.erase(std::find_if(objects.begin(), objects.end(), collisionObjectsComparator));

    DiscardEmptyNodes();
}

template<typename T>
void Quadtree<T>::Node::DiscardEmptyNodes()
{
    if (!objects.empty())
    {
        return;
    }

    if (childs[0] != nullptr)
    {
        for (const Node* node : childs)
        {
            if (node->childs[0] != nullptr || !node->objects.empty())
            {
                return;
            }
        }
    }

    Clear();
    if (parent != nullptr)
    {
        parent->DiscardEmptyNodes();
    }
}

template<typename T>
void Quadtree<T>::Node::Insert(ObjectBBoxCollision&& collisionObject)
{
    if (childs[0] != nullptr)
    {
        int32_t childIndex = GetIndex(collisionObject.bbox);

        if (childIndex != -1)
        {
            childs[childIndex]->Insert(std::move(collisionObject));

            return;
        }
    }

    objects.push_back(collisionObject);

    if (objects.size() > kMaxObjectsInNode)
    {
        if (childs[0] == nullptr)
        {
            Split();
        }

        size_t i = 0;
        while (i < objects.size())
        {
            int32_t childIndex = GetIndex(objects[i].bbox);
            if (childIndex != -1)
            {
                childs[childIndex]->Insert(std::move(objects[i]));
                objects.erase(objects.begin() + i);
            }
            else
            {
                ++i;
            }
        }
    }
}

template<typename T>
void Quadtree<T>::Node::DebugDraw(const Renderer* renderer)
{
    std::vector<Vector2> drawLines;
    drawLines.reserve(8 * 4);

    for (size_t i = 0; i < childs.size(); ++i) {
        Node* child = childs[i];
        if (child != nullptr) {
            child->DebugDraw(renderer);

            drawLines.push_back(child->bounds.min);
            drawLines.push_back({ child->bounds.min.x, child->bounds.max.y });

            drawLines.push_back({ child->bounds.min.x, child->bounds.max.y });
            drawLines.push_back(child->bounds.max);

            drawLines.push_back(child->bounds.max);
            drawLines.push_back({ child->bounds.max.x, child->bounds.min.y });

            drawLines.push_back({ child->bounds.max.x, child->bounds.min.y });
            drawLines.push_back(child->bounds.min);
        }
    }

    renderer->DrawLines(drawLines);
}

template<typename T>
void Quadtree<T>::DebugDraw(const Renderer* renderer)
{
    rootNode->DebugDraw(renderer);
}

template<typename T>
void Quadtree<T>::Node::Clear()
{
    objects.clear();

    for (size_t i = 0; i < childs.size(); ++i) {
        if (childs[i] != nullptr) {
            childs[i]->Clear();
            delete childs[i];
            childs[i] = nullptr;
        }
    }
}

template<typename T>
void Quadtree<T>::Node::Split()
{
    Vector2 bboxHalfSize = bounds.GetSize() / 2.0f;

    Vector2 firstQuarterMin = bounds.min + bboxHalfSize;
    Vector2 secondQuarterMin = bounds.min + Vector2{ 0.0f, bboxHalfSize.y};
    Vector2 thirdQuarterMin = bounds.min;
    Vector2 fourthQuarterMin = bounds.min + Vector2{ bboxHalfSize.x, 0.0f };

    childs[0] = new Node(this, { firstQuarterMin, firstQuarterMin + bboxHalfSize});
    childs[1] = new Node(this, { secondQuarterMin, secondQuarterMin + bboxHalfSize});
    childs[2] = new Node(this, { thirdQuarterMin, thirdQuarterMin + bboxHalfSize});
    childs[3] = new Node(this, { fourthQuarterMin, fourthQuarterMin + bboxHalfSize});
}

template<typename T>
Quadtree<T>::Quadtree(const AABBox2& bounds, const std::function<AABBox2(const T&)>& aBBoxCreationFunc)
    : bboxCreationFunc(aBBoxCreationFunc)
{
    rootNode = new Node(nullptr, bounds);
}

template<typename T>
void Quadtree<T>::Clear()
{
    rootNode->Clear();
}
