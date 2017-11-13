#pragma once

class IBehavior
{
public:
    virtual void OnAwake()  = 0;
    virtual void OnUpdate() = 0;
    virtual void OnExit()   = 0;
};

class ITransformBehavior : public IBehavior
{
public:
    virtual void GetParent() = 0;
    virtual void GetChildren() = 0;
};

class IRenderer 
{
public:
    virtual void Draw() = 0;
    virtual void SetVertexShader() = 0;
    virtual void GetVertexShader() = 0;
    virtual void SetPixelShader() = 0;
    virtual void GetPixelShader() = 0;
};

class Actor : IBehavior
{
public:
    void AddRenderer(){};
    void AddCompornent() {};
    void Draw() {};
private:
    // Actor * children;
    // Actor * parent;
    // list<rederer> m_renderer;
    // list<ibehabeor> m_compornents;
};