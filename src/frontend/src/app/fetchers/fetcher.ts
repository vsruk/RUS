export async function fetcher<T>(
  input: string | URL | globalThis.Request,
  init?: RequestInit
): Promise<T> {
  const headerJSON = localStorage.getItem("ezgrada-header");
  if (headerJSON == null) throw new Error("Problem while accessing user data");
  const header = JSON.parse(headerJSON);
  const response = await fetch(input, {
    headers: {
      "Content-Type": "application/json",
      Accept: "aplication/json",
      "access-token": header.accessToken,
      client: header.client,
      "token-type": header.tokenType,
      uid: header.uid,
      expiry: header.expiry,
    },
    ...init,
  });
  if (!response.ok) {
    throw new Error(`Dogodila se greška kod fetchanja`);
  }
  const data = await response.json();
  return data;
}
