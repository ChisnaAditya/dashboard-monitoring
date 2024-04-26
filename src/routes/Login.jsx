import MyInput from "../components/MyInput";
import MyButton from "../components/MyButton";
import { useState } from "react";
import { useNavigate } from "react-router-dom";
import { signInWithEmailAndPassword } from "firebase/auth";
import { auth } from "../firebase";

export default function Login() {
  const [input, setInput] = useState({
    email: "",
    password: "",
  });
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const navigate = useNavigate();

  const handleInput = (e) => {
    const { name, value } = e.target;
    setInput((prev) => ({
      ...prev,
      [name]: value,
    }));
  };
  const handleSubmit = (e) => {
    e.preventDefault();
    signInWithEmailAndPassword(auth, input.email, input.password)
      .then((userCredential) => {
        console.log(userCredential);
        navigate("/");
      })
      .catch((error) => {
        console.log(error);
      });
  };

  return (
    <div className="flex flex-col items-center justify-center min-h-screen mx-auto bg-pattern">
      <h1 className="text-4xl text-indigo-600 font-bold py-5">Login</h1>
      <form
        onSubmit={handleSubmit}
        className="container space-y-4 border rounded-md p-[2rem] w-[300px] bg-white shadow-lg"
      >
        <MyInput
          for="email"
          inputName="Email"
          id="email"
          name="email"
          type="email"
          onChange={handleInput}
        />
        <MyInput
          for="password"
          inputName="Password"
          id="password"
          name="password"
          type="password"
          onChange={handleInput}
        />
        <MyButton type="submit" buttonName="Login" />
        <div className="text-sm text-center py-2">
          <a
            href="/forgot"
            className="font-semibold text-indigo-600 hover:text-indigo-500"
          >
            Forgot Password?
          </a>
        </div>
      </form>
    </div>
  );
}
