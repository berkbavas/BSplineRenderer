#pragma once

#include "Curve/Spline.h"

#include <QStack>
#include <QVector3D>
#include <functional>

namespace BSplineRenderer
{
    // Base class for Undo/Redo commands
    class Command
    {
      public:
        virtual ~Command() = default;
        virtual void Execute() = 0;
        virtual void Undo() = 0;
        virtual QString GetDescription() const = 0;
    };

    using CommandPtr = std::shared_ptr<Command>;

    // Knot position change command
    class MoveKnotCommand : public Command
    {
      public:
        MoveKnotCommand(KnotPtr knot, const QVector3D& oldPos, const QVector3D& newPos, SplinePtr spline)
            : mKnot(knot)
            , mOldPosition(oldPos)
            , mNewPosition(newPos)
            , mSpline(spline)
        {
        }

        void Execute() override
        {
            mKnot->SetPosition(mNewPosition);
            if (mSpline)
                mSpline->MakeDirty();
        }

        void Undo() override
        {
            mKnot->SetPosition(mOldPosition);
            if (mSpline)
                mSpline->MakeDirty();
        }

        QString GetDescription() const override { return "Move Knot"; }

      private:
        KnotPtr mKnot;
        QVector3D mOldPosition;
        QVector3D mNewPosition;
        SplinePtr mSpline;
    };

    // Add knot command
    class AddKnotCommand : public Command
    {
      public:
        AddKnotCommand(SplinePtr spline, KnotPtr knot)
            : mSpline(spline)
            , mKnot(knot)
        {
        }

        void Execute() override
        {
            mSpline->AddKnot(mKnot);
        }

        void Undo() override
        {
            mSpline->RemoveLastKnot();
        }

        QString GetDescription() const override { return "Add Knot"; }

      private:
        SplinePtr mSpline;
        KnotPtr mKnot;
    };

    // Undo/Redo manager
    class UndoRedoManager
    {
      public:
        static UndoRedoManager& Instance()
        {
            static UndoRedoManager instance;
            return instance;
        }

        void ExecuteCommand(CommandPtr command)
        {
            command->Execute();
            mUndoStack.push(command);
            mRedoStack.clear();
        }

        void Undo()
        {
            if (CanUndo())
            {
                auto command = mUndoStack.pop();
                command->Undo();
                mRedoStack.push(command);
            }
        }

        void Redo()
        {
            if (CanRedo())
            {
                auto command = mRedoStack.pop();
                command->Execute();
                mUndoStack.push(command);
            }
        }

        bool CanUndo() const { return !mUndoStack.isEmpty(); }
        bool CanRedo() const { return !mRedoStack.isEmpty(); }

        int UndoCount() const { return mUndoStack.size(); }
        int RedoCount() const { return mRedoStack.size(); }

        void Clear()
        {
            mUndoStack.clear();
            mRedoStack.clear();
        }

        QString GetUndoDescription() const
        {
            return mUndoStack.isEmpty() ? "" : mUndoStack.top()->GetDescription();
        }

        QString GetRedoDescription() const
        {
            return mRedoStack.isEmpty() ? "" : mRedoStack.top()->GetDescription();
        }

      private:
        UndoRedoManager() = default;

        QStack<CommandPtr> mUndoStack;
        QStack<CommandPtr> mRedoStack;
    };
}
