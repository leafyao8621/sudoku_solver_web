import React, { useState } from "react";
import axios from "axios";
import { useLocation } from "react-router-dom";
import {
    Button,
    Form,
    FormGroup,
    Input,
    Label,
} from "reactstrap";

const LandingPage = (props) => {
    const location  = useLocation();
    const [blockRow, setBlockRow] = useState(3);
    const [blockColumn, setBlockColumn] = useState(3);
    const [selectedRow, setSelectedRow] = useState(0);
    const [selectedColumn, setSelectedColumn] = useState(0);
    const [data, setData] =
        useState(
            () => {
                return (
                    Array.from(
                        Array(9).keys()
                    ).map(
                        () => {
                            return (
                                Array.from(
                                    Array(9).keys()
                                ).map(() => null)
                            )
                        }
                    )
                );
            }
        );
    const clear = () => {
        setData(
            (
                () => {
                    return (
                        Array.from(
                            Array(blockRow * blockColumn).keys()
                        ).map(
                            () => {
                                return (
                                    Array.from(
                                        Array(
                                            blockRow *
                                            blockColumn
                                        ).keys()
                                    ).map(() => null)
                                )
                            }
                        )
                    );
                }
            )()
        );
    };
    const solve = () => {
        const payload =
            {
                block_row: blockRow,
                block_column: blockColumn,
                data: data
            }
        axios.post(
            `http://${window.location.hostname}:8000/solve`,
            payload
        ).then((res) => {
            if (!res.data.success) {
                alert("Error!");
            } else {
                setData(res.data.result);
                alert("Success!");
            }
        }).catch((err) => {
            alert(err);
        })
    };
    const renderGrid = () => {
        return (
            <table>
                <tbody>
                {
                    Array.from(
                        Array(blockColumn * blockRow).keys()
                    ).map(
                        (i) => {
                            return (
                                <tr key={`inpr${i}`}>
                                    {
                                        Array.from(
                                            Array(blockColumn * blockRow).keys()
                                        ).map(
                                            (j) => {
                                                return (
                                                    <td key={`inp${i}${j}`}>
                                                        <Button
                                                            outline color=
                                                            {
                                                                i === selectedRow &&
                                                                j === selectedColumn ?
                                                                "warning" :
                                                                "primary"
                                                            }
                                                            onClick=
                                                            {
                                                                () => {
                                                                    setSelectedRow(i);
                                                                    setSelectedColumn(j);
                                                                }
                                                            }
                                                        >
                                                            {
                                                                data[i][j] ===
                                                                null ?
                                                                (
                                                                    blockColumn * blockRow > 9 ?
                                                                    "XX" :
                                                                    "X"
                                                                ) :
                                                                (
                                                                    blockColumn * blockRow > 9 ?
                                                                    `${data[i][j]}`.padStart(2, "0") :
                                                                    data[i][j]
                                                                )
                                                            }
                                                        </Button>
                                                    </td>
                                                )
                                            }
                                        )
                                    }
                                </tr>
                            )
                        }
                    )
                }
                </tbody>
            </table>
        )
    };
    const renderInput = () => {
        return (
            <table>
                <tbody>
                {
                    Array.from(
                        Array(blockRow).keys()
                    ).map((i) => {
                        return (
                            <tr key={`contr${i}`}>
                                {
                                    Array.from(
                                        Array(blockColumn).keys()
                                    ).map((j) => {
                                        return (
                                            <td key={`cont${i}${j}`}>
                                            <Button
                                                onClick=
                                                {
                                                    () => {
                                                        setData(
                                                            Array.from(
                                                                data.keys()
                                                            ).map((k) => {
                                                                return (
                                                                    Array.from(
                                                                        data[j].keys()
                                                                    ).map((l) => {
                                                                        return (
                                                                            k === selectedRow &&
                                                                            l === selectedColumn ?
                                                                            i * blockColumn + j + 1 :
                                                                            data[k][l]
                                                                        );
                                                                    })
                                                                );
                                                            })
                                                        );
                                                    }
                                                }
                                            >
                                                {
                                                    blockColumn * blockRow > 9 ?
                                                    `${i * blockColumn + j + 1}`.padStart(2, "0") :
                                                    i * blockColumn + j + 1
                                                }
                                            </Button>
                                        </td>
                                        )
                                    })
                                }
                            </tr>
                        )
                    })
                }
                <tr>
                    <td>
                        <Button
                            onClick=
                            {
                                () => {
                                    setData(
                                        Array.from(
                                            data.keys()
                                        ).map((j) => {
                                            return (
                                                Array.from(
                                                    data[j].keys()
                                                ).map((k) => {
                                                    return (
                                                        j === selectedRow &&
                                                        k === selectedColumn ?
                                                        null :
                                                        data[j][k]
                                                    );
                                                })
                                            );
                                        })
                                    );
                                }
                            }
                        >
                            {
                                blockColumn * blockRow > 9 ?
                                "XX" :
                                "X"
                            }
                        </Button>
                    </td>
                </tr>
                </tbody>
            </table>
        )
    };
    return (
        <main className="container">
            <h1>Logged in as { location.state.userName }</h1>
            <Form>
                <FormGroup>
                    <Label>Block Rows</Label>
                    <Input
                        type="number"
                        max="4"
                        min="1"
                        defaultValue="3"
                        onChange={({ target }) => {
                            const row = parseInt(target.value);
                            setBlockRow(row);
                            setData(
                                (
                                    () => {
                                        return (
                                            Array.from(
                                                Array(row * blockColumn).keys()
                                            ).map(
                                                () => {
                                                    return (
                                                        Array.from(
                                                            Array(
                                                                row *
                                                                blockColumn
                                                            ).keys()
                                                        ).map(() => null)
                                                    )
                                                }
                                            )
                                        );
                                    }
                                )()
                            );
                        }}
                    />
                    <Label>Block Columns</Label>
                    <Input
                        type="number"
                        max="4"
                        min="1"
                        defaultValue="3"
                        onChange={({ target }) => {
                            const column = parseInt(target.value);
                            setBlockColumn(column);
                            setData(
                                (
                                    () => {
                                        return (
                                            Array.from(
                                                Array(blockRow * column).keys()
                                            ).map(
                                                () => {
                                                    return (
                                                        Array.from(
                                                            Array(
                                                                blockRow * column
                                                            ).keys()
                                                        ).map(() => null)
                                                    )
                                                }
                                            )
                                        );
                                    }
                                )()
                            );
                        }}
                    />
                </FormGroup>
            </Form>
            <Button onClick={ solve }>Solve</Button>
            <Button onClick={ clear }>Clear</Button>
            <table>
                <tbody>
                <tr>
                    <td>
                        { renderGrid() }
                    </td>
                    <td>
                        { renderInput() }
                    </td>
                </tr>
                </tbody>
            </table>
        </main>
    );
};

export default LandingPage;
