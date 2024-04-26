import { RouterProvider, createBrowserRouter } from "react-router-dom";
import Dashboard from "./routes/Dashboard";
import Login from "./routes/Login";
import Forgot from "./routes/Forgot";
import NotFound from "./routes/NotFound";
import "./App.css";

function App() {
  const router = createBrowserRouter([
    {
      path: "/",
      element: <Dashboard />,
    },
    {
      path: "/login",
      element: <Login />,
    },
    {
      path: "/forgot",
      element: <Forgot />,
    },
    {
      path: "*",
      element: <NotFound />,
    },
  ]);
  return (
    <>
      <RouterProvider router={router} />
    </>
  );
}

export default App;
