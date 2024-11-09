"use client";
import { Field } from "@/components/ui/field";
import { InputGroup } from "@/components/ui/input-group";
import { PasswordInput } from "@/components/ui/password-input";
import { Button } from "@/components/ui/button";
import { Flex, Text, Input, Heading } from "@chakra-ui/react";
import { useForm } from "react-hook-form";
import { GoogleLogin } from "@react-oauth/google";
import useSWRMutation from "swr/mutation";
import { loginMutator } from "@/fetchers/mutators";
import { swrKeys } from "@/typings/swrKeys";
import { useRouter } from "next/navigation";
import useSWR from "swr";

interface ILoginForm {
  email: string;
  password: string;
}

export const LoginForm = () => {
  const router = useRouter();
  const {
    register,
    handleSubmit,
    setError,
    formState: { isSubmitting, errors },
  } = useForm<ILoginForm>();
  function delay(time: number) {
    return new Promise((resolve) => setTimeout(resolve, time));
  }

  const { mutate } = useSWR("login");
  const { trigger } = useSWRMutation(swrKeys.login, loginMutator, {
    onSuccess: async (data) => {
      const loginInfo = {
        token: data.token,
        role: data.role,
      };
      localStorage.setItem("loginInfo", JSON.stringify(loginInfo));
      console.log("info: ", loginInfo);

      await mutate(data);
      if (loginInfo.role == "Administrator") router.push("/create");
      else router.push("home");
    },
    onError: async (error: { message: string }) => {
      console.log(error.message);
      setError("password", { message: error.message });
    },
  });
  const onCreate = async (data: ILoginForm) => {
    try {
      await trigger(data);
    } catch (err) {}
  };

  return (
    <>
      <Flex
        padding="20px"
        borderRadius="10px"
        bg="gray.100"
        direction="column"
        alignItems="center"
        as="form"
        height="100%"
        width="50%"
        gapY="10px"
        onSubmit={handleSubmit(onCreate)}
      >
        <Heading marginBottom="20px" fontSize="2xl">
          Login
        </Heading>
        <Field
          label="Email"
          invalid={Boolean(errors?.email)}
          disabled={isSubmitting}
          errorText={errors?.email?.message}
        >
          <Input {...register("email", { required: "Unesite svoj email" })} />
        </Field>
        <Field
          label="Lozinka"
          invalid={Boolean(errors?.password)}
          disabled={isSubmitting}
          errorText={errors?.password?.message}
        >
          <PasswordInput
            {...register("password", { required: "Unesite lozinku" })}
          />
        </Field>
        <Button
          color="white"
          bg="black"
          marginTop="20px"
          type="submit"
          width="100%"
          loading={isSubmitting}
        >
          Log In
        </Button>
        <Text> ili </Text>
        <GoogleLogin
          text="signin_with"
          onSuccess={(credentialResponse) => {
            console.log(credentialResponse);
          }}
          onError={() => {
            console.log("Login Failed");
          }}
        />
      </Flex>
    </>
  );
};
